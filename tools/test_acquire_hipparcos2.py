"""Focused acquisition tests: band selection, catalogue identity, and snapshot bounds."""

import csv
import gzip
import importlib.util
import json
from pathlib import Path
import sqlite3
import tempfile
import unittest

spec = importlib.util.spec_from_file_location("acquire_hipparcos2", Path(__file__).with_name("acquire_hipparcos2.py"))
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


def hip_line(hip, hp, bv="0.009"):
    line = list(" " * 276)
    for start, end, value in (
        (0, 6, str(hip)), (15, 28, "1.7678185359"), (29, 42, "-0.2916993748"),
        (43, 50, "379.21"), (51, 59, "-546.01"), (60, 68, "-1223.07"),
        (129, 136, str(hp)), (152, 158, bv)):
        line[start:end] = value.rjust(end - start)
    return "".join(line) + "\n"


class AcquisitionTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)

    def tearDown(self):
        self.temp.cleanup()

    def test_native_selection_strict_boundary_and_missing_colour(self):
        path = self.root / "hip.gz"
        with gzip.open(path, "wt") as stream:
            stream.write(hip_line(1, "2.0000"))
            stream.write(hip_line(32349, "-1.0876"))
            stream.write(hip_line(2, "1.0000", ""))
        rows, counts = module.read_hipparcos(path, 2.0)
        self.assertEqual([r["hip_id"] for r in rows], ["32349"])
        self.assertEqual(rows[0]["hp_mag"], "-1.0876")
        self.assertEqual(rows[0]["b_v"], "0.009")
        self.assertEqual(rows[0]["ref_epoch"], "1991.25")
        self.assertAlmostEqual(float(rows[0]["ra"]), 101.28854, places=4)
        self.assertAlmostEqual(float(rows[0]["dec"]), -16.713143, places=4)
        self.assertEqual(counts["incomplete_native_rows"], 1)

    def test_duplicate_hip_fails(self):
        path = self.root / "hip.gz"
        with gzip.open(path, "wt") as stream:
            stream.write(hip_line(1, "1.0000") * 2)
        with self.assertRaisesRegex(ValueError, "duplicate"):
            module.read_hipparcos(path, 2)

    def test_votable_rejects_overflow_even_after_initial_ok(self):
        path = self.root / "result.xml"
        path.write_text('<VOTABLE><RESOURCE><INFO name="QUERY_STATUS" value="OK"/>'
                        '<TABLE><FIELD name="source_id"/><DATA><TABLEDATA>'
                        '<TR><TD>9007199254740993</TD></TR></TABLEDATA></DATA></TABLE>'
                        '<INFO name="QUERY_STATUS" value="OVERFLOW"/></RESOURCE></VOTABLE>')
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
            db.execute("CREATE TABLE ids (id INTEGER PRIMARY KEY, row_number INTEGER NOT NULL)")
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

    def test_deterministic_gzip_and_empty_measurements(self):
        rows = [{"hip_id": "1", "gaia_g": ""}]
        a, b = self.root / "a.gz", self.root / "b.gz"
        module.write_csv(a, ["hip_id", "gaia_g"], rows)
        module.write_csv(b, ["hip_id", "gaia_g"], rows)
        self.assertEqual(a.read_bytes(), b.read_bytes())


if __name__ == "__main__":
    unittest.main()
