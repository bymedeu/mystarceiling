#!/usr/bin/env python3
"""Acquire a bounded, ordered Gaia DR3 CSV.gz; no application-loader code."""

import argparse
import csv
import datetime
import fcntl
import gzip
import hashlib
import io
import json
import math
import os
from pathlib import Path
import shlex
import shutil
import sqlite3
import struct
import subprocess
import time
import xml.etree.ElementTree as ET

FIELDS = "source_id ref_epoch ra dec pmra pmdec parallax phot_g_mean_mag bp_rp".split()
ENDPOINT = "https://gea.esac.esa.int/tap-server/tap/async"
BENCHMARKS = (4096, 65536, 262144, 1048576)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--root", type=Path, required=True)
    ap.add_argument("--target-rows", type=int, default=134_217_728)
    ap.add_argument("--max-bytes", type=int, default=10_000_000_000)
    ap.add_argument("--initial-job")
    ap.add_argument("--initial-query", type=Path)
    ap.add_argument("--initial-result", type=Path)
    args = ap.parse_args()
    if args.target_rows <= 0:
        ap.error("target-rows must be positive")
    if args.max_bytes <= 0:
        ap.error("max-bytes must be positive")
    root = args.root.resolve()
    root.mkdir(parents=True, exist_ok=True)
    work = root / ".acquisition"
    work.mkdir(exist_ok=True)
    lockfile = (work / "acquire.lock").open("w")
    try:
        fcntl.flock(lockfile, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except BlockingIOError as error:
        raise RuntimeError(
            "Another Gaia acquisition process is already running"
        ) from error
    log = root / "acquisition-log.md"
    statepath = work / "state.json"
    archive = root / "gaia-dr3.csv.gz.partial"
    complete = root / "gaia-dr3.csv.gz"
    if complete.exists():
        raise RuntimeError("Completed archive already exists; refusing overwrite")

    def record(message):
        with log.open("a") as f:
            f.write(message + "\n")

    def report(message):
        print(
            datetime.datetime.now(datetime.timezone.utc).isoformat(),
            message,
            flush=True,
        )

    def curl(options, *, limit=60):
        command = [
            "curl",
            "--fail",
            "--silent",
            "--show-error",
            "--location",
            "--max-time",
            str(limit),
        ] + options
        # Do not automatically follow the POST->303 submission below: GET job next.
        if "--dump-header" in options:
            command.remove("--location")
        for attempt in range(3):
            record(
                "\n### "
                + datetime.datetime.now(datetime.timezone.utc).isoformat()
                + "\n\n```sh\n"
                + shlex.join(command)
                + "\n```"
            )
            result = subprocess.run(command, capture_output=True, text=True)
            record("Exit code: " + str(result.returncode))
            if result.returncode == 0:
                return
            record("```text\n" + result.stderr + "\n```")
            # Retrying submissions can create orphan jobs; leave them to manual recovery.
            if "--dump-header" in options:
                break
            if attempt < 2:
                time.sleep(10)
        raise RuntimeError("Transfer failed; see acquisition-log.md")

    def save(state):
        tmp = statepath.with_suffix(".new")
        tmp.write_text(json.dumps(state, indent=2) + "\n")
        os.replace(tmp, statepath)

    if statepath.exists():
        state = json.loads(statepath.read_text())
        if state["max_bytes"] != args.max_bytes:
            raise RuntimeError("Resume cap differs")
        if state.get("target_rows") not in (None, args.target_rows):
            raise RuntimeError("Resume row target differs")
        if state["rows"] > args.target_rows:
            raise RuntimeError("Resume already exceeds row target")
        if state.get("target_rows") is None:
            state["target_rows"] = args.target_rows
            save(state)
    else:
        state = {
            "max_bytes": args.max_bytes,
            "target_rows": args.target_rows,
            "rows": 0,
            "bytes": 0,
            "batch": 0,
            "cursor": None,
            "job": args.initial_job,
            "job_started": bool(args.initial_job),
            "job_limit": 1048576 if args.initial_job else 3000000,
            "done": False,
            "benchmarks": {},
            "rejected_rows": 0,
        }
        record(
            "# Gaia DR3 acquisition log\n\nAll nine fields required. No added quality cuts.\n"
            "Order: ascending G magnitude, then source_id. Final gzip size cap: "
            + str(args.max_bytes)
            + " bytes. Exact row target: "
            + str(args.target_rows)
            + ". "
            "Gzip members contain up to 4096 CSV rows; stop before a member would "
            "exceed the cap. This is not the complete Gaia catalogue.\n"
        )
        if args.initial_query:
            record(
                "## Previously submitted initial query\n\n```sql\n"
                + args.initial_query.read_text()
                + "```"
            )
        save(state)
    invocation = os.environ.get("MSC_GAIA_INVOCATION")
    if not invocation:
        invocation = shlex.join(
            ["python3", "-B", str(Path(__file__).resolve())] + os.sys.argv[1:]
        )
    record("\n## Invocation\n\n```sh\n" + invocation + "\n```")
    # Recover an interrupted append: the JSON checkpoint is the commit boundary.
    if archive.exists():
        with archive.open("r+b") as f:
            f.truncate(state["bytes"])
    elif state["bytes"]:
        raise RuntimeError("Missing partial archive")
    db = sqlite3.connect(work / "ids.sqlite3")
    db.execute(
        "CREATE TABLE IF NOT EXISTS ids (id INTEGER PRIMARY KEY, row_number INTEGER NOT NULL)"
    )
    db.execute("DELETE FROM ids WHERE row_number > ?", (state["rows"],))
    db.commit()
    if state["rows"] == args.target_rows:
        state["done"] = True
        state["stop_reason"] = "target row count"
        save(state)
    while not state["done"]:
        if shutil.disk_usage(root).free < 2_000_000_000:
            raise RuntimeError(
                "Less than 2 GB free for bounded working files; resume after freeing disk"
            )
        if not state["job"]:
            query_limit = min(
                state.get("job_limit", 3000000), args.target_rows - state["rows"]
            )
            query = (
                f"SELECT TOP {query_limit} "
                + ", ".join(FIELDS)
                + "\nFROM gaiadr3.gaia_source\nWHERE "
            )
            query += "\n  AND ".join(f + " IS NOT NULL" for f in FIELDS)
            if state["cursor"]:
                mag, sid = state["cursor"]
                query += f"\n  AND (phot_g_mean_mag > {mag!r} OR (phot_g_mean_mag = {mag!r} AND source_id > {sid}))"
            query += "\nORDER BY phot_g_mean_mag ASC, source_id ASC\n"
            qfile = work / f"batch-{state['batch']:05d}.adql"
            qfile.write_text(query)
            record(
                "\n## Batch "
                + str(state["batch"])
                + " query\n\n```sql\n"
                + query
                + "```"
            )
            headers = work / "submit.headers"
            curl(
                [
                    "--dump-header",
                    str(headers),
                    "--output",
                    str(work / "submit.body"),
                    "--data-urlencode",
                    "REQUEST=doQuery",
                    "--data-urlencode",
                    "LANG=ADQL",
                    "--data-urlencode",
                    "FORMAT=csv",
                    "--data-urlencode",
                    f"MAXREC={query_limit}",
                    "--data-urlencode",
                    "QUERY@" + str(qfile),
                    ENDPOINT,
                ],
                limit=300,
            )
            locations = [
                line.split(":", 1)[1].strip()
                for line in headers.read_text().splitlines()
                if line.lower().startswith("location:")
            ]
            if not locations or not locations[-1].startswith(ENDPOINT + "/"):
                raise RuntimeError("No trusted job URL")
            state.update(job=locations[-1], job_started=False, job_limit=query_limit)
            save(state)
        job = state["job"]
        if not state["job_started"]:
            curl(
                [
                    "--output",
                    str(work / "start.body"),
                    "--data-urlencode",
                    "PHASE=RUN",
                    job + "/phase",
                ]
            )
            state["job_started"] = True
            save(state)
        while True:
            xmlfile = work / "job.xml"
            curl(["--output", str(xmlfile), job])
            tree = ET.parse(xmlfile)
            phase = next(e.text for e in tree.iter() if e.tag.split("}")[-1] == "phase")
            report(
                f"Batch {state['batch']}: {phase}; committed {state['rows']:,} rows / {state['bytes']:,} bytes"
            )
            if phase == "COMPLETED":
                break
            if phase in ("ERROR", "ABORTED"):
                curl(["--output", str(work / "error.xml"), job + "/error"])
                error_text = (work / "error.xml").read_text()
                if (
                    "Maximum execution time" in error_text
                    and state["job_limit"] > 250000
                ):
                    old_limit = state["job_limit"]
                    state.update(
                        job=None,
                        job_started=False,
                        job_limit=max(250000, old_limit // 2),
                    )
                    save(state)
                    record(
                        f"\nESA timed out batch {state['batch']} at {old_limit} rows; "
                        f"retrying from the unchanged cursor with {state['job_limit']} rows."
                    )
                    report(
                        f"Batch {state['batch']}: timed out; retrying with "
                        f"{state['job_limit']:,} rows"
                    )
                    break
                raise RuntimeError("ESA job failed; see .acquisition/error.xml")
            time.sleep(30)
        if state["job"] is None:
            continue
        result = next(e for e in tree.iter() if e.tag.split("}")[-1] == "result")
        url = result.attrib.get("{http://www.w3.org/1999/xlink}href")
        if not url or not url.startswith("https://gea.esac.esa.int/"):
            raise RuntimeError("Unexpected result URL")
        raw = work / "batch.csv"
        if args.initial_result and state["batch"] == 0 and args.initial_result.exists():
            shutil.copyfile(args.initial_result, raw)
            record("Reused initial result: " + str(args.initial_result))
        else:
            curl(["--output", str(raw), url], limit=1800)
        batch_count = 0
        previous = None
        committed_cursor = tuple(state["cursor"]) if state["cursor"] else None
        with raw.open("rb") as probe:
            compressed = probe.read(2) == b"\x1f\x8b"
        opener = gzip.open if compressed else open
        with opener(raw, "rt", newline="") as f:
            reader = csv.reader(f)
            if next(reader, None) != FIELDS:
                raise RuntimeError("Unexpected CSV schema")
            buffer = []
            ids = []
            keys = []
            buffer_last = None

            def commit_buffer():
                nonlocal buffer, ids, keys, buffer_last
                if not buffer:
                    return True
                remaining = args.target_rows - state["rows"]
                if remaining <= 0:
                    state["done"] = True
                    state["stop_reason"] = "target row count"
                    save(state)
                    return False
                if len(buffer) > remaining:
                    buffer = buffer[:remaining]
                    ids = ids[:remaining]
                    keys = keys[:remaining]
                buffer_last = keys[-1]
                textio = io.StringIO(newline="")
                writer = csv.writer(textio, lineterminator="\n")
                if state["rows"] == 0:
                    writer.writerow(FIELDS)
                writer.writerows(buffer)
                member = gzip.compress(
                    textio.getvalue().encode(), compresslevel=6, mtime=0
                )
                if state["bytes"] + len(member) > args.max_bytes:
                    raise RuntimeError(
                        "Exact row target cannot fit under compressed byte cap"
                    )
                # IDs are exact integers. A uniqueness violation is acquisition failure.
                db.executemany(
                    "INSERT INTO ids VALUES (?,?)",
                    ((sid, state["rows"] + i + 1) for i, sid in enumerate(ids)),
                )
                db.commit()
                with archive.open("ab") as out:
                    out.write(member)
                    out.flush()
                    os.fsync(out.fileno())
                state["rows"] += len(buffer)
                state["bytes"] += len(member)
                state["cursor"] = list(buffer_last)
                if state["rows"] in BENCHMARKS:
                    state["benchmarks"][str(state["rows"])] = {
                        "gzip_prefix_bytes": state["bytes"]
                    }
                if state["rows"] == args.target_rows:
                    state["done"] = True
                    state["stop_reason"] = "target row count"
                save(state)
                buffer = []
                ids = []
                keys = []
                return not state["done"]

            for row in reader:
                batch_count += 1
                if len(row) != 9 or any(x == "" for x in row):
                    raise RuntimeError("Malformed/incomplete archive row")
                sid = int(row[0])
                values = [float(x) for x in row[1:]]
                if not 0 < sid < 2**63 or not all(math.isfinite(x) for x in values):
                    raise RuntimeError("Invalid numeric source data")
                if not 0 <= values[1] < 360 or not -90 <= values[2] <= 90:
                    raise RuntimeError("Invalid coordinate range")
                # Gaia's G column is float32. Recover its exact value for keyset paging;
                # rounded CSV decimals must not cause repeated/skipped magnitude ties.
                mag = struct.unpack("f", struct.pack("f", float(row[7])))[0]
                key = (mag, sid)
                if previous is not None and key <= previous:
                    raise RuntimeError("Non-increasing magnitude/ID order")
                previous = key
                # On resume, re-fetch this same job and skip its committed prefix.
                if committed_cursor is not None and key <= committed_cursor:
                    continue
                buffer_last = key
                buffer.append(row)
                ids.append(sid)
                keys.append(key)
                if len(buffer) == 4096 and not commit_buffer():
                    break
            if not state["done"]:
                commit_buffer()
        if not state["done"]:
            if batch_count > state["job_limit"]:
                raise RuntimeError("Unexpected row overflow")
            if batch_count < state["job_limit"]:
                state["done"] = True
                state["stop_reason"] = "source exhausted"
            state.update(job=None, job_started=False, batch=state["batch"] + 1)
            save(state)
        record(
            f"\nValidated batch: {batch_count} inspected rows. Total committed: {state['rows']}; gzip bytes: {state['bytes']}."
        )
        report(f"Committed {state['rows']:,} sources, {state['bytes']:,} bytes")
        raw.unlink()
    db.close()
    if state["rows"] < max(BENCHMARKS):
        raise RuntimeError("Catalogue does not contain all agreed benchmark sizes")
    report("Checking gzip integrity and prefix hashes")
    prefix_hash = hashlib.sha256()
    hashes = {}
    n = 0
    with gzip.open(archive, "rb") as f:
        header = f.readline()
        prefix_hash.update(header)
        for line in f:
            n += 1
            prefix_hash.update(line)
            if n in BENCHMARKS:
                hashes[str(n)] = prefix_hash.hexdigest()
    if n != state["rows"]:
        raise RuntimeError("Final row-count mismatch")
    digest = hashlib.sha256()
    with archive.open("rb") as f:
        for block in iter(lambda: f.read(8 * 1024 * 1024), b""):
            digest.update(block)
    manifest = {
        "release": "Gaia DR3",
        "columns": FIELDS,
        "rows": n,
        "compressed_bytes": archive.stat().st_size,
        "target_rows": args.target_rows,
        "max_bytes": args.max_bytes,
        "sha256": digest.hexdigest(),
        "prefix_csv_sha256": hashes,
        "order": ["phot_g_mean_mag ASC", "source_id ASC"],
        "stop_reason": state["stop_reason"],
        "last_key": state["cursor"],
        "format": "CSV in concatenated gzip members; one header",
        "utc_completed": datetime.datetime.now(datetime.timezone.utc).isoformat(),
    }
    (root / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    os.replace(archive, complete)
    record(
        "\n## Verified result\n\n```json\n" + json.dumps(manifest, indent=2) + "\n```"
    )
    (work / "ids.sqlite3").unlink()
    report("COMPLETE " + str(complete))


if __name__ == "__main__":
    main()
