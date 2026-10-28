"""Focused acquisition tests: band selection, catalogue identity, and snapshot bounds."""

import contextlib
import io
import sys
from unittest.mock import patch

import csv
import gzip
import importlib.util
import json
import math
from pathlib import Path
import sqlite3
import tempfile
import unittest

spec = importlib.util.spec_from_file_location(
    "acquire_hipparcos2", Path(__file__).with_name("acquire_hipparcos2.py")
)
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


def hip_line(hip, hp, bv="0.009", vi="0.013"):
    line = list(" " * 276)
    for start, end, value in (
        (0, 6, str(hip)),
        (15, 28, "1.7678185359"),
        (29, 42, "-0.2916993748"),
        (43, 50, "379.21"),
        (51, 59, "-546.01"),
        (60, 68, "-1223.07"),
        (129, 136, str(hp)),
        (152, 158, bv),
        (165, 171, vi),
    ):
        line[start:end] = value.rjust(end - start)
    return "".join(line) + "\n"


class AcquisitionTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)

    def tearDown(self):
        self.temp.cleanup()

    def fake_fetch(self, run, log, record, name, url, query=None):
        target = run / name
        if name == "hip2.dat.gz":
            with gzip.open(target, "wt") as stream:
                stream.write(hip_line(1, "1.0000"))
        elif name.endswith(".xml"):
            fields = (
                ["match_count"]
                if name.startswith("match-count")
                else module.MATCH + module.GAIA
            )
            columns = "".join(f'<FIELD name="{field}"/>' for field in fields)
            rows = "<TR><TD>0</TD></TR>" if name.startswith("match-count") else ""
            target.write_text(
                '<VOTABLE><RESOURCE><INFO name="QUERY_STATUS" value="OK"/>'
                f"<TABLE>{columns}<DATA><TABLEDATA>{rows}</TABLEDATA>"
                "</DATA></TABLE></RESOURCE></VOTABLE>"
            )
        else:
            target.write_text("Synthetic ReadMe")
        return target

    def run_offline(self, args):
        with contextlib.ExitStack() as stack:
            stack.enter_context(patch.object(sys, "argv", args))
            stack.enter_context(patch.object(module, "EXPECTED_HIP_ROWS", 1))
            stack.enter_context(patch.object(module, "fetch", self.fake_fetch))
            stack.enter_context(
                patch.object(
                    module.subprocess, "run", side_effect=AssertionError("network")
                )
            )
            stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            module.main()

    def test_main_publishes_supplement_and_snapshot_manifest_offline(self):
        gaia_root = self.root / "gaia"
        gaia_root.mkdir()
        archive = gaia_root / "gaia-dr3.csv.gz"
        with gzip.open(archive, "wt") as stream:
            stream.write(",".join(module.GAIA) + "\n")
        (gaia_root / "manifest.json").write_text(
            json.dumps({"rows": 0, "compressed_bytes": archive.stat().st_size})
        )
        output = self.root / "hipparcos"
        args = [
            "acquire_hipparcos2.py",
            "--root",
            str(output),
            "--gaia-root",
            str(gaia_root),
            "--g-limit",
            "2",
        ]
        self.run_offline(args)
        run = output / json.loads((output / "latest.json").read_text())["run"]
        manifest = json.loads((run / "manifest.json").read_text())
        self.assertEqual(manifest["counts"]["selected_rows"], 1)
        self.assertEqual(manifest["gaia_snapshot"]["kind"], "completed_archive")
        self.assertEqual(
            manifest["resolution_counts"], {"no_published_best_neighbour": 1}
        )
        with gzip.open(run / "hipparcos2-supplement.csv.gz", "rt") as stream:
            rows = list(csv.DictReader(stream))
        self.assertEqual(rows[0]["hip_id"], "1")
        self.assertEqual(rows[0]["resolution_reason"], "no_published_best_neighbour")
        self.assertEqual(
            manifest["files"]["hipparcos2-supplement.csv.gz"]["sha256"],
            module.digest(run / "hipparcos2-supplement.csv.gz"),
        )

    def test_derived_g_selection_and_missing_colour(self):
        path = self.root / "hip.gz"
        with gzip.open(path, "wt") as stream:
            stream.write(hip_line(1, "3.0000"))
            stream.write(hip_line(32349, "-1.0876"))
            stream.write(hip_line(2, "1.0000", ""))
        rows, counts = module.read_hipparcos(path, 2.0)
        self.assertEqual([r["hip_id"] for r in rows], ["32349"])
        self.assertEqual(rows[0]["hp_mag"], "-1.0876")
        self.assertEqual(rows[0]["b_v"], "0.009")
        self.assertEqual(rows[0]["v_i"], "0.013")
        self.assertEqual(rows[0]["g_estimation_method"], "edr3_hp_bv")
        self.assertAlmostEqual(float(rows[0]["estimated_g"]), -1.1151784321)
        self.assertEqual(rows[0]["ref_epoch"], "1991.25")
        self.assertAlmostEqual(float(rows[0]["ra"]), 101.28854, places=4)
        self.assertAlmostEqual(float(rows[0]["dec"]), -16.713143, places=4)
        self.assertEqual(counts["incomplete_native_rows"], 1)

    def test_uses_vi_fallback_outside_unambiguous_bv_range(self):
        estimate = module.estimate_gaia_g(1.0, 1.5, 1.2)
        self.assertEqual(estimate[1], "edr3_hp_vi_fallback")
        self.assertAlmostEqual(estimate[0], 0.4715432)

    def test_rejects_colours_outside_both_supported_ranges(self):
        self.assertIsNone(module.estimate_gaia_g(1.0, -0.3, 5.0))

    def test_resolution_prefers_only_complete_locally_downloaded_gaia(self):
        native = [{field: "1" for field in module.PREPARED}]
        complete = {field: "1" for field in module.GAIA + module.MATCH}
        complete["original_ext_source_id"] = "1"
        supplement, counts = module.resolve_supplement(native, [complete], {1})
        self.assertEqual(supplement[0]["gaia_source_id"], "1")
        self.assertEqual(counts["prefer_complete_local_gaia_if_selected"], 1)

        incomplete = complete | {"pmra": ""}
        supplement, counts = module.resolve_supplement(native, [incomplete], set())
        self.assertEqual(
            supplement[0]["resolution_reason"], "published_gaia_missing_required_fields"
        )
        self.assertEqual(counts["published_gaia_missing_required_fields"], 1)

    def test_resolution_retains_unmatched_hipparcos_candidate(self):
        native = [{field: "1" for field in module.PREPARED}]
        supplement, counts = module.resolve_supplement(native, [], set())
        self.assertEqual(
            supplement[0]["resolution_reason"], "no_published_best_neighbour"
        )
        self.assertEqual(counts["no_published_best_neighbour"], 1)

    def test_inclusive_bright_range_and_order_exposes_old_cutoff(self):
        path = self.root / "hip.gz"
        with gzip.open(path, "wt") as stream:
            stream.write(hip_line(3, "3.0000"))
            stream.write(hip_line(2, "3.0000"))
        boundary = module.estimate_gaia_g(3.0, 0.009, 0.013)[0]
        rows, _ = module.read_hipparcos(path, boundary)
        self.assertEqual([r["hip_id"] for r in rows], ["2", "3"])
        self.assertEqual(
            module.read_hipparcos(path, math.nextafter(boundary, -float("inf")))[0],
            [],
        )
        self.assertEqual(len(module.read_hipparcos(path, module.DEFAULT_G_LIMIT)[0]), 2)
        self.assertEqual(module.read_hipparcos(path, 1.9425238)[0], [])

    def test_nonlocal_and_faint_neighbour_cannot_suppress_primary(self):
        native = [{field: "1" for field in module.PREPARED}]
        link = {field: "1" for field in module.GAIA + module.MATCH}
        link["original_ext_source_id"] = "1"
        rows, _ = module.resolve_supplement(native, [link], set())
        self.assertEqual(rows[0]["gaia_source_id"], "0")
        self.assertEqual(
            rows[0]["resolution_reason"],
            "complete_gaia_not_in_verified_local_selection",
        )
        rows, _ = module.resolve_supplement(
            native, [link | {"phot_g_mean_mag": "6.8278775"}], {1}
        )
        self.assertEqual(rows[0]["gaia_source_id"], "0")
        self.assertEqual(
            rows[0]["resolution_reason"], "published_gaia_outside_bright_range"
        )
        self.assertEqual(rows[0]["excluded_gaia_source_id"], "1")
        rows, _ = module.resolve_supplement(
            native, [link | {"phot_g_mean_mag": "6"}], {1}
        )
        self.assertEqual(rows[0]["gaia_source_id"], "1")

    def test_default_limit_and_explicit_override(self):
        args = [
            "acquire",
            "--root",
            str(self.root / "hip"),
            "--gaia-root",
            str(self.root / "gaia"),
        ]
        with patch.object(sys, "argv", args):
            self.assertEqual(module.arguments()[0].g_limit, 6.0)
        with patch.object(sys, "argv", args + ["--g-limit", "1.9425238"]):
            self.assertEqual(module.arguments()[0].g_limit, 1.9425238)

    def test_real_bright_landmarks_from_raw_through_resolution(self):
        fixture = Path(__file__).resolve().parent.parent / "tests/fixtures/hipparcos2"
        raw = self.root / "hip2.gz"
        with gzip.open(raw, "wb") as stream:
            stream.write((fixture / "hip2-bright.dat").read_bytes())
        rows, counts = module.read_hipparcos(raw, module.DEFAULT_G_LIMIT)
        self.assertEqual(counts["selected_rows"], 7)
        matches = module.read_votable(
            fixture / "matches-0000.xml", module.MATCH + module.GAIA
        )
        module.validate_matches(matches, {int(r["hip_id"]) for r in rows})
        prepared, _ = module.resolve_supplement(rows, matches, set())
        output = self.root / "prepared.gz"
        module.write_csv(output, module.SUPPLEMENT, prepared)
        self.assertEqual(
            output.read_bytes(), (fixture / "prepared.csv.gz").read_bytes()
        )
        by_id = {int(r["hip_id"]): r for r in prepared}
        for hip, g in (
            (14576, 2.0742011124304),
            (25930, 2.185029),
            (58001, 2.3868610055232),
        ):
            self.assertEqual(float(by_id[hip]["estimated_g"]), g)
            self.assertEqual(by_id[hip]["gaia_source_id"], "0")
        self.assertEqual(
            by_id[25930]["resolution_reason"], "published_gaia_missing_required_fields"
        )
        self.assertEqual(len(module.read_hipparcos(raw, 1.9425238)[0]), 4)

    def test_duplicate_hip_fails(self):
        path = self.root / "hip.gz"
        with gzip.open(path, "wt") as stream:
            stream.write(hip_line(1, "1.0000") * 2)
        with self.assertRaisesRegex(ValueError, "duplicate"):
            module.read_hipparcos(path, 2)

    def test_votable_rejects_overflow_even_after_initial_ok(self):
        path = self.root / "result.xml"
        path.write_text(
            '<VOTABLE><RESOURCE><INFO name="QUERY_STATUS" value="OK"/>'
            '<TABLE><FIELD name="source_id"/><DATA><TABLEDATA>'
            "<TR><TD>9007199254740993</TD></TR></TABLEDATA></DATA></TABLE>"
            '<INFO name="QUERY_STATUS" value="OVERFLOW"/></RESOURCE></VOTABLE>'
        )
        with self.assertRaisesRegex(ValueError, "truncated"):
            module.read_votable(path, ["source_id"])
        path.write_text(path.read_text().replace('value="OVERFLOW"', 'value="OK"'))
        rows = module.read_votable(path, ["source_id"])
        self.assertEqual(int(rows[0]["source_id"]), 9007199254740993)

    def test_checkpoint_excludes_uncommitted_ids_without_mutation(self):
        work = self.root / ".acquisition"
        work.mkdir()
        state = {"rows": 1, "bytes": 1, "cursor": [1, 11]}
        (work / "state.json").write_text(json.dumps(state))
        (self.root / "gaia-dr3.csv.gz.partial").write_bytes(b"x")
        index = work / "ids.sqlite3"
        with sqlite3.connect(index) as db:
            db.execute(
                "CREATE TABLE ids (id INTEGER PRIMARY KEY, row_number INTEGER NOT NULL)"
            )
            db.executemany("INSERT INTO ids VALUES (?, ?)", [(11, 1), (22, 2)])
        before = module.digest(index)
        found, snapshot = module.local_membership(self.root, {11, 22, 33})
        self.assertEqual(found, {11})
        self.assertEqual(snapshot["kind"], "incomplete_checkpoint")
        self.assertEqual(module.digest(index), before)

    def test_completed_catalogue_membership_and_count_validation(self):
        archive = self.root / "gaia-dr3.csv.gz"
        with gzip.open(archive, "wt", newline="") as stream:
            writer = csv.writer(stream)
            writer.writerow(module.GAIA)
            writer.writerow([99, 2016, 0, 0, 0, 0, 0, 1, 0])
        manifest = {"rows": 1, "compressed_bytes": archive.stat().st_size}
        path = self.root / "manifest.json"
        path.write_text(json.dumps(manifest))
        found, _ = module.local_membership(self.root, {99, 100})
        self.assertEqual(found, {99})
        manifest["rows"] = 2
        path.write_text(json.dumps(manifest))
        with self.assertRaisesRegex(ValueError, "count"):
            module.local_membership(self.root, {99})

    def test_completed_membership_proves_only_declared_inclusive_prefix(self):
        archive = self.root / "gaia-dr3.csv.gz"
        with gzip.open(archive, "wt", newline="") as stream:
            writer = csv.writer(stream)
            writer.writerow(module.GAIA)
            for sid, g in ((10, 1), (20, 6), (30, 7)):
                writer.writerow([sid, 2016, 0, 0, 0, 0, 0, g, 0])
        (self.root / "manifest.json").write_text(
            json.dumps({"rows": 3, "compressed_bytes": archive.stat().st_size})
        )
        found, evidence = module.local_membership(self.root, {10, 20, 30}, 6)
        self.assertEqual(found, {10, 20})
        self.assertEqual(evidence["verification_scope"], "ordered bright prefix")
        self.assertEqual(evidence["rows_scanned"], 3)
        with self.assertRaisesRegex(ValueError, "unordered"):
            module.validated_gaia_key(
                ["10", "2016", "0", "0", "0", "0", "0", "1", "0"], (2, 20)
            )

    def test_deterministic_gzip_and_empty_measurements(self):
        rows = [{"hip_id": "1", "gaia_g": ""}]
        a, b = self.root / "a.gz", self.root / "b.gz"
        module.write_csv(a, ["hip_id", "gaia_g"], rows)
        module.write_csv(b, ["hip_id", "gaia_g"], rows)
        self.assertEqual(a.read_bytes(), b.read_bytes())


if __name__ == "__main__":
    unittest.main()
