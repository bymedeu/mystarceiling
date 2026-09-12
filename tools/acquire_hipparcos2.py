#!/usr/bin/env python3
"""Download a separate, native Hipparcos-2 Hp-selected catalogue and Gaia links.

Only Python's standard library and curl are required. No photometric conversion,
epoch propagation, positional matching, Gaia writes, or application integration.
"""

import argparse
from collections import Counter
import csv
import datetime as dt
import fcntl
import gzip
import hashlib
import io
import json
import math
from pathlib import Path
import shlex
import sqlite3
import subprocess
import sys
import xml.etree.ElementTree as ET

CDS = "https://cdsarc.cds.unistra.fr/ftp/I/311/"
TAP = "https://gaia.aip.de/tap/sync"
NATIVE = "hip_id ref_epoch ra dec pmra pmdec parallax hp_mag b_v".split()
GAIA = "source_id ref_epoch ra dec pmra pmdec parallax phot_g_mean_mag bp_rp".split()
MATCH = "original_ext_source_id angular_distance number_of_neighbours xm_flag".split()
EXPECTED_HIP_ROWS = 117955


def digest(path):
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def finite(text):
    return bool(text) and math.isfinite(float(text))


def read_hipparcos(path, hp_limit):
    """CDS I/311 hip2.dat fixed byte columns; retain native photometric bands."""
    selected = []
    seen = set()
    counts = Counter()
    with gzip.open(path, "rt", encoding="ascii", newline="") as stream:
        for line_number, line in enumerate(stream, 1):
            line = line.rstrip("\r\n")
            if len(line) != 276:
                raise ValueError(f"Hipparcos line {line_number}: expected 276 bytes")
            hip = int(line[0:6])
            if hip <= 0 or hip in seen:
                raise ValueError(f"Invalid or duplicate HIP identifier: {hip}")
            seen.add(hip)
            counts["source_rows"] += 1
            # RA/Dec are radians in the source, at Julian epoch 1991.25.
            values = [line[a:b].strip() for a, b in (
                (15, 28), (29, 42), (51, 59), (60, 68),
                (43, 50), (129, 136), (152, 158))]
            if not all(finite(v) for v in values):
                counts["incomplete_native_rows"] += 1
                continue
            ra, dec, pmra, pmdec, parallax, hp, bv = values
            if not (0 <= float(ra) < 2 * math.pi and
                    -math.pi / 2 <= float(dec) <= math.pi / 2):
                raise ValueError(f"Invalid Hipparcos coordinates for HIP {hip}")
            counts["complete_native_rows"] += 1
            if float(hp) >= hp_limit:
                counts["outside_hp_selection"] += 1
                continue
            selected.append(dict(zip(NATIVE, (
                str(hip), "1991.25", repr(math.degrees(float(ra))),
                repr(math.degrees(float(dec))), pmra, pmdec, parallax, hp, bv))))
    selected.sort(key=lambda row: (float(row["hp_mag"]), int(row["hip_id"])))
    counts["selected_rows"] = len(selected)
    return selected, dict(counts)


def read_votable(path, expected_fields):
    root = ET.parse(path).getroot()
    statuses = [node.get("value") for node in root.iter()
                if node.tag.split("}")[-1] == "INFO"
                and node.get("name") == "QUERY_STATUS"]
    if not statuses or any(value != "OK" for value in statuses):
        raise ValueError(f"TAP query failed or truncated: {statuses}")
    tables = [node for node in root.iter() if node.tag.split("}")[-1] == "TABLE"]
    if len(tables) != 1:
        raise ValueError("Expected exactly one TAP result table")
    table = tables[0]
    fields = [node.get("name") for node in table
              if node.tag.split("}")[-1] == "FIELD"]
    if fields != expected_fields:
        raise ValueError(f"Unexpected TAP fields: {fields}")
    data = [node for node in table.iter()
            if node.tag.split("}")[-1] == "TABLEDATA"]
    if len(data) != 1:
        raise ValueError("Expected VOTable TABLEDATA encoding")
    rows = []
    for node in data[0]:
        cells = [(cell.text or "").strip() for cell in node]
        if len(cells) != len(fields):
            raise ValueError("Wrong TAP row width")
        rows.append(dict(zip(fields, cells)))
    return rows


def match_query(hip_ids, count=False):
    ids = ",".join(str(int(hip)) for hip in sorted(hip_ids))
    where = f" WHERE x.original_ext_source_id IN ({ids})"
    if count:
        return "SELECT COUNT(*) AS match_count FROM gaiadr3.hipparcos2_best_neighbour AS x" + where
    columns = ["x." + field for field in MATCH] + ["g." + field for field in GAIA]
    return ("SELECT " + ", ".join(columns) +
            " FROM gaiadr3.hipparcos2_best_neighbour AS x"
            " LEFT OUTER JOIN gaiadr3.gaia_source AS g ON x.source_id = g.source_id" +
            where + " ORDER BY x.original_ext_source_id ASC, g.source_id ASC")


def validate_matches(rows, hip_ids):
    seen = set()
    for row in rows:
        hip = int(row["original_ext_source_id"])
        if hip not in hip_ids:
            raise ValueError("Cross-match contains an unrequested Hipparcos ID")
        sid = int(row["source_id"])
        if sid <= 0 or (hip, sid) in seen:
            raise ValueError("Invalid or duplicate cross-match pair")
        seen.add((hip, sid))
        for field in MATCH[1:] + GAIA[1:]:
            if row[field] and not finite(row[field]):
                raise ValueError(f"Nonfinite Gaia/cross-match field: {field}")


def local_membership(gaia_root, source_ids):
    """Read a recorded Gaia checkpoint and its existing ID index, never write it.

    Row-number bounding excludes index entries newer than the checkpoint.
    A completed catalogue is streamed instead, because acquisition deletes its
    temporary index. Missing local data is an error, not evidence of absence.
    """
    complete = gaia_root / "gaia-dr3.csv.gz"
    if complete.exists():
        manifest_path = gaia_root / "manifest.json"
        manifest = json.loads(manifest_path.read_text())
        if complete.stat().st_size != manifest["compressed_bytes"]:
            raise ValueError("Completed Gaia size disagrees with its manifest")
        found = set()
        count = 0
        with gzip.open(complete, "rt", encoding="ascii", newline="") as stream:
            reader = csv.reader(stream)
            if next(reader) != GAIA:
                raise ValueError("Unexpected Gaia header")
            for row in reader:
                count += 1
                if len(row) != len(GAIA):
                    raise ValueError("Malformed completed Gaia row")
                sid = int(row[0])
                if sid in source_ids:
                    found.add(sid)
        if count != manifest["rows"]:
            raise ValueError("Completed Gaia count disagrees with its manifest")
        return found, {"kind": "completed_archive", "path": str(complete),
                       "rows": count, "manifest": manifest,
                       "manifest_sha256": digest(manifest_path)}

    state_path = gaia_root / ".acquisition/state.json"
    state_bytes = state_path.read_bytes()
    state = json.loads(state_bytes)
    archive = gaia_root / "gaia-dr3.csv.gz.partial"
    if archive.stat().st_size < state["bytes"]:
        raise ValueError("Gaia archive is shorter than its checkpoint")
    index = gaia_root / ".acquisition/ids.sqlite3"
    db = sqlite3.connect(index.resolve().as_uri() + "?mode=ro", uri=True, timeout=10)
    try:
        db.execute("PRAGMA query_only=ON")
        db.execute("BEGIN")
        last = db.execute("SELECT MAX(row_number) FROM ids").fetchone()[0]
        if (last or 0) < state["rows"]:
            raise ValueError("Gaia ID index is behind its checkpoint; retry")
        found = set()
        for sid in source_ids:
            result = db.execute("SELECT row_number FROM ids WHERE id = ?", (sid,)).fetchone()
            if result and 0 < result[0] <= state["rows"]:
                found.add(sid)
    finally:
        db.close()
    return found, {"kind": "incomplete_checkpoint", "path": str(archive),
                   "checkpoint": state,
                   "checkpoint_sha256": hashlib.sha256(state_bytes).hexdigest(),
                   "membership_evidence": "read-only acquisition ID index, bounded by checkpoint row count"}


def write_csv(path, fields, rows):
    # Deterministic gzip: no wall time or output filename in the gzip header.
    with path.open("wb") as raw:
        with gzip.GzipFile(fileobj=raw, mode="wb", filename="", mtime=0) as compressed:
            with io.TextIOWrapper(compressed, encoding="ascii", newline="") as text:
                writer = csv.DictWriter(text, fieldnames=fields, lineterminator="\n")
                writer.writeheader()
                writer.writerows(rows)
    with gzip.open(path, "rt", encoding="ascii", newline="") as stream:
        reader = csv.DictReader(stream)
        if reader.fieldnames != fields or list(reader) != rows:
            raise ValueError(f"CSV round-trip verification failed: {path}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, required=True)
    parser.add_argument("--gaia-root", type=Path, required=True)
    parser.add_argument("--hp-limit", type=float, required=True,
                        help="Strict native Hp cutoff; never interpreted as Gaia G")
    parser.add_argument("--tap-endpoint", default=TAP)
    args = parser.parse_args()
    if not math.isfinite(args.hp_limit):
        parser.error("hp-limit must be finite")
    root, gaia_root = args.root.resolve(), args.gaia_root.resolve()
    if root == gaia_root or root.is_relative_to(gaia_root) or gaia_root.is_relative_to(root):
        parser.error("Hipparcos and Gaia roots must be separate directories")
    root.mkdir(parents=True, exist_ok=True)
    lock = (root / ".acquire.lock").open("a")
    fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
    started = dt.datetime.now(dt.timezone.utc)
    run = root / "runs" / started.strftime("%Y%m%dT%H%M%S.%fZ")
    run.mkdir(parents=True)
    log = run / "acquisition-log.md"
    log.write_text("# Hipparcos-2 acquisition\n\n```sh\n" +
                   shlex.join([sys.executable, *sys.argv]) + "\n```\n")

    def record(text):
        with log.open("a") as stream:
            stream.write(text + "\n")

    def fetch(name, url, query=None):
        target = run / name
        command = ["curl", "--fail", "--silent", "--show-error", "--location",
                   "--max-time", "120", "--output", str(target)]
        if query is not None:
            (run / (name + ".adql")).write_text(query + "\n")
            command += ["--get", "--data-urlencode", "REQUEST=doQuery",
                        "--data-urlencode", "LANG=ADQL", "--data-urlencode", "FORMAT=votable",
                        "--data-urlencode", "MAXREC=1000000", "--data-urlencode", "QUERY=" + query]
        command.append(url)
        record("\n## " + dt.datetime.now(dt.timezone.utc).isoformat() +
               "\n\n```sh\n" + shlex.join(command) + "\n```")
        result = subprocess.run(command, capture_output=True, text=True)
        record(f"Exit code: {result.returncode}\n\n```text\n{result.stderr}\n```")
        if result.returncode:
            raise RuntimeError(f"Download failed; see {log}. Previous completed runs are intact.")
        return target

    print(f"Acquiring native Hipparcos-2 Hp < {args.hp_limit}; output: {run}", flush=True)
    fetch("ReadMe", CDS + "ReadMe")
    hip = fetch("hip2.dat.gz", CDS + "hip2.dat.gz")
    selected, counts = read_hipparcos(hip, args.hp_limit)
    if counts["source_rows"] != EXPECTED_HIP_ROWS:
        raise ValueError("Hipparcos-2 source count changed; inspect before accepting")
    write_csv(run / "hipparcos2.csv.gz", NATIVE, selected)
    hip_ids = {int(row["hip_id"]) for row in selected}
    matches = []
    if hip_ids:
        count_file = fetch("match-count.xml", args.tap_endpoint, match_query(hip_ids, count=True))
        count_rows = read_votable(count_file, ["match_count"])
        if len(count_rows) != 1:
            raise ValueError("Expected one cross-match count")
        match_file = fetch("matches.xml", args.tap_endpoint, match_query(hip_ids))
        matches = read_votable(match_file, MATCH + GAIA)
        if len(matches) != int(count_rows[0]["match_count"]):
            raise ValueError("Cross-match row count mismatch")
        validate_matches(matches, hip_ids)
    found, snapshot = local_membership(gaia_root, {int(row["source_id"]) for row in matches})
    by_hip = {hip: [] for hip in hip_ids}
    for row in matches:
        by_hip[int(row["original_ext_source_id"])].append(row)
    output = []
    status_counts = Counter()
    fields = ["hip_id", "local_status"] + ["gaia_" + field for field in GAIA] + MATCH[1:]
    for native in selected:
        hip_id = native["hip_id"]
        links = by_hip[int(hip_id)]
        if not links:
            status = "no_published_best_neighbour"
            output.append(dict(zip(fields, [hip_id, status] + [""] * (len(fields) - 2))))
            status_counts[status] += 1
        for link in links:
            status = ("matched_locally_downloaded" if int(link["source_id"]) in found
                      else "matched_not_locally_downloaded")
            output.append(dict(zip(fields, [hip_id, status] +
                                   [link[field] for field in GAIA] +
                                   [link[field] for field in MATCH[1:]])))
            status_counts[status] += 1
    write_csv(run / "gaia-crossmatch.csv.gz", fields, output)
    manifest = {
        "format_version": 1, "release": "Hipparcos-2 (van Leeuwen 2007), CDS I/311",
        "utc_started": started.isoformat(),
        "utc_completed": dt.datetime.now(dt.timezone.utc).isoformat(),
        "script_sha256": digest(Path(__file__)), "source_url": CDS,
        "selection": {"expression": "hp_mag < hp_limit", "hp_limit": args.hp_limit,
                      "band": "Hipparcos Hp", "required_native_columns": NATIVE,
                      "provisional": True, "equivalent_to_gaia_g_cut": False},
        "counts": counts, "crossmatch_status_row_counts": dict(status_counts),
        "crossmatch_endpoint": args.tap_endpoint,
        "crossmatch_table": "gaiadr3.hipparcos2_best_neighbour",
        "gaia_snapshot": snapshot,
        "units": {"ra": "degrees ICRS", "dec": "degrees ICRS",
                  "ref_epoch": "Julian year 1991.25", "pmra": "mas/yr, RA motion with cos(dec)",
                  "pmdec": "mas/yr", "parallax": "mas", "hp_mag": "Hipparcos Hp mag",
                  "b_v": "B minus V mag", "angular_distance": "arcsec"},
        "order": ["hp_mag ASC", "hip_id ASC"],
        "limitations": ["Hp cutoff is not a Gaia G cutoff.",
                        "No photometric conversion or position propagation was performed.",
                        "No published best neighbour does not establish absence from Gaia.",
                        "Local Gaia membership is a snapshot, not a final fusion decision.",
                        "Native source flags, errors and other fields remain in hip2.dat.gz.",
                        "Multiple links are retained; no ambiguous identity is silently resolved."],
        "files": {path.name: {"bytes": path.stat().st_size, "sha256": digest(path)}
                  for path in sorted(run.iterdir()) if path.is_file() and path != log},
    }
    record("\n## Validated outcome\n\n```json\n" + json.dumps(manifest, indent=2) + "\n```")
    (run / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    pointer = root / "latest.json.new"
    pointer.write_text(json.dumps({"run": str(run.relative_to(root))}, indent=2) + "\n")
    pointer.replace(root / "latest.json")
    print(json.dumps({"run": str(run), "counts": counts, "matches": dict(status_counts)}, indent=2))


if __name__ == "__main__":
    main()
