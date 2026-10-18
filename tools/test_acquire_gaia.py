"""Offline Gaia recovery tests. Network and cookie values are synthetic fixtures."""

import contextlib
import gc
import gzip
import importlib.util
import io
import json
from pathlib import Path
import sqlite3
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch
import warnings
from xml.sax.saxutils import escape

SPEC = importlib.util.spec_from_file_location(
    "acquire_gaia", Path(__file__).with_name("acquire_gaia.py")
)
gaia = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(gaia)
URL = gaia.ENDPOINT + "/"
HEADER = ",".join(gaia.FIELDS) + "\n"


class RecoveryTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="msc-gaia-test-")
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name) / "data"
        self.work = self.root / ".acquisition"
        self.work.mkdir(parents=True)
        # This is not an ESA session. Never read an operational cookie jar in tests.
        self.jar = Path(self.temporary.name) / "cookies"
        self.jar.write_text("# synthetic fixture\n")
        self.jar.chmod(0o600)
        self.archive = self.root / "gaia-dr3.csv.gz.partial"
        self.member = gzip.compress((HEADER + "1,2016,1,1,1,1,1,1,1\n").encode())
        self.archive.write_bytes(self.member)
        self.initial = dict(
            max_bytes=10_000_000_000, target_rows=10, rows=1, bytes=len(self.member),
            batch=26, cursor=[1.0, 1], job=URL + "old", job_started=True,
            job_limit=2, done=False, benchmarks={}, rejected_rows=0,
        )
        self.write_state(self.initial)
        with contextlib.closing(sqlite3.connect(self.work / "ids.sqlite3")) as db:
            db.execute("CREATE TABLE ids(id INTEGER PRIMARY KEY,row_number INTEGER NOT NULL)")
            db.execute("INSERT INTO ids VALUES(1,1)")
            db.commit()
        self.calls = []
        self.mode = "ok"
        self.query = ""
        self.duration = 60
        self.phase = "PENDING"

    def state(self):
        return json.loads((self.work / "state.json").read_text())

    def write_state(self, state):
        (self.work / "state.json").write_text(json.dumps(state))

    def ids(self):
        with contextlib.closing(sqlite3.connect(self.work / "ids.sqlite3")) as db:
            return db.execute("SELECT id,row_number FROM ids ORDER BY row_number").fetchall()

    def fake_curl(self, command, **kwargs):
        self.calls.append(command)
        output = Path(command[command.index("--output") + 1])
        endpoint = command[-1]
        code = 0
        if "--dump-header" in command:
            self.query = (self.work / "batch-00026.adql").read_text()
            Path(command[command.index("--dump-header") + 1]).write_text(
                "HTTP/1.1 303\nSet-Cookie: SYNTHETIC_SESSION_VALUE\nLocation: " + URL + "new\n"
            )
            output.write_text("")
            if self.mode == "submission-timeout":
                code = 28
        elif endpoint.endswith("/executionduration"):
            self.duration = 60 if self.mode == "quota" else 600
            output.write_text("")
        elif endpoint.endswith("/phase"):
            if self.mode == "crash-before-run":
                raise RuntimeError("interrupted before RUN")
            self.phase = "COMPLETED"
            output.write_text("")
            if self.mode == "lost-run-response":
                self.phase = "EXECUTING"
                code = 28
        elif "--request" in command:
            self.assertNotIn("--location", command)
            output.write_text("")
            if self.mode == "delete-failure":
                code = 22
        elif endpoint.endswith("/results/result"):
            if self.mode == "expired-download":
                return subprocess.CompletedProcess(command, 22, "", "HTTP 401")
            rows = "2,2016,1,1,1,1,1,2,1\n3,2016,1,1,1,1,1,3,1\n"
            if self.mode == "duplicate":
                rows = "1,2016,1,1,1,1,1,2,1\n3,2016,1,1,1,1,1,3,1\n"
            if self.mode == "short":
                rows = rows.splitlines(True)[0]
            if self.mode == "overflow":
                rows += "4,2016,1,1,1,1,1,4,1\n"
            output.write_text(HEADER + rows)
        elif endpoint == URL + "old":
            phase = "EXECUTING" if self.mode == "old-running" else "ABORTED"
            output.write_text(
                f"<job><phase>{phase}</phase>"
                "<message>Maximum execution time (60 s) reached</message></job>"
            )
        else:
            owner = "anonymous" if self.mode == "anonymous" else "fixture_user"
            query = self.query + " changed" if self.mode == "query" else self.query
            maxrec = 1 if self.mode == "maxrec" else 2
            output.write_text(
                f"<job><phase>{self.phase}</phase><ownerId>{owner}</ownerId>"
                f"<executionDuration>{self.duration}</executionDuration><parameters>"
                f'<parameter id="query">{escape(query)}</parameter>'
                f'<parameter id="maxrec">{maxrec}</parameter></parameters>'
                f'<results><result xmlns:xlink="http://www.w3.org/1999/xlink" '
                f'xlink:href="{URL}new/results/result"/></results></job>'
            )
            if self.phase == "EXECUTING":
                self.phase = "COMPLETED"
        return subprocess.CompletedProcess(command, code, "", "fixture transfer failed" if code else "")

    def run_acquisition(self, *, replace=True, before_replace=None):
        args = [
            "acquire_gaia.py", "--root", str(self.root), "--target-rows", "10",
            "--max-bytes", str(self.initial["max_bytes"]), "--cookie-jar", str(self.jar),
            "--max-batches", "1",
        ]
        if replace:
            args += ["--replace-aborted-job", URL + "old"]
        with contextlib.ExitStack() as stack:
            stack.enter_context(patch.object(sys, "argv", args))
            stack.enter_context(patch.object(gaia.subprocess, "run", self.fake_curl))
            stack.enter_context(patch.object(gaia.time, "sleep"))
            stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            # Main owns these resources for its process lifetime. Keep exception
            # paths in one test process without ResourceWarning noise.
            stack.enter_context(warnings.catch_warnings())
            warnings.simplefilter("ignore", ResourceWarning)
            if before_replace:
                real_replace = gaia.os.replace

                def interrupted_replace(source, destination):
                    before_replace(Path(source), Path(destination))
                    real_replace(source, destination)

                stack.enter_context(patch.object(gaia.os, "replace", interrupted_replace))
            try:
                gaia.main()
            finally:
                gc.collect()

    def assert_initial_commit(self):
        self.assertEqual(self.state()["rows"], 1)
        self.assertEqual(self.state()["bytes"], len(self.member))
        self.assertEqual(self.state()["cursor"], [1.0, 1])
        self.assertEqual(self.archive.read_bytes(), self.member)

    def assert_advanced(self):
        state = self.state()
        self.assertEqual(state["rows"], 3)
        self.assertGreater(state["bytes"], len(self.member))
        self.assertEqual(state["bytes"], self.archive.stat().st_size)
        self.assertEqual(state["cursor"], [3.0, 3])
        self.assertEqual(state["batch"], 27)
        self.assertEqual(self.ids(), [(1, 1), (2, 2), (3, 3)])
        self.assertEqual(len(gzip.decompress(self.archive.read_bytes()).splitlines()), 4)

    def test_committed_advancement_query_and_secret_header_redaction(self):
        self.run_acquisition()
        self.assert_advanced()
        expected = (
            "SELECT TOP 2 " + ", ".join(gaia.FIELDS)
            + "\nFROM gaiadr3.gaia_source\nWHERE "
            + "\n  AND ".join(field + " IS NOT NULL" for field in gaia.FIELDS)
            + "\n  AND (phot_g_mean_mag > 1.0 OR (phot_g_mean_mag = 1.0 AND source_id > 1))"
            + "\nORDER BY phot_g_mean_mag ASC, source_id ASC\n"
        )
        self.assertEqual(self.query, expected)
        for path in [self.root / "acquisition-log.md", self.work / "submit.headers"]:
            self.assertNotIn("SYNTHETIC_SESSION_VALUE", path.read_text())
        for command in self.calls:
            self.assertEqual(command[1], "--disable")
            self.assertEqual(command[command.index("--cookie") + 1], str(self.jar))
        self.assertEqual(sum("--dump-header" in c for c in self.calls), 1)

    def test_preflight_failures_preserve_old_checkpoint_and_never_run(self):
        for mode in ["anonymous", "quota", "query", "maxrec", "old-running"]:
            with self.subTest(mode=mode):
                self.mode = mode
                with self.assertRaises(RuntimeError):
                    self.run_acquisition()
                self.assertEqual(self.state(), self.initial)
                self.assert_initial_commit()
                self.assertFalse(any(c[-1].endswith("/phase") for c in self.calls))
                self.calls.clear()

    def test_submission_timeout_is_not_retried(self):
        self.mode = "submission-timeout"
        with self.assertRaises(RuntimeError):
            self.run_acquisition()
        self.assertEqual(sum("--dump-header" in c for c in self.calls), 1)
        self.assertEqual(self.state(), self.initial)

    def test_lost_run_response_adopts_existing_job_without_second_run(self):
        self.mode = "lost-run-response"
        with self.assertRaises(RuntimeError):
            self.run_acquisition()
        self.assertEqual(self.state()["job"], URL + "new")
        self.assertFalse(self.state()["job_started"])
        self.assert_initial_commit()
        self.mode = "ok"
        self.run_acquisition(replace=False)
        self.assert_advanced()
        self.assertEqual(sum(c[-1].endswith("/phase") for c in self.calls), 1)
        self.assertEqual(sum("--dump-header" in c for c in self.calls), 1)

    def test_saved_pending_candidate_is_revalidated_on_resume(self):
        self.mode = "crash-before-run"
        with self.assertRaisesRegex(RuntimeError, "before RUN"):
            self.run_acquisition()
        self.assertEqual(self.phase, "PENDING")
        self.assertEqual(self.state()["job"], URL + "new")
        self.assertFalse(self.state()["job_started"])
        self.mode = "anonymous"
        with self.assertRaisesRegex(RuntimeError, "authenticated"):
            self.run_acquisition(replace=False)
        self.assert_initial_commit()
        self.mode = "ok"
        self.run_acquisition(replace=False)
        self.assert_advanced()

    def test_append_before_json_commit_is_repaired_without_duplicates(self):
        def crash(source, destination):
            if json.loads(source.read_text())["rows"] > 1:
                raise RuntimeError("interrupted JSON commit")
        with self.assertRaisesRegex(RuntimeError, "JSON commit"):
            self.run_acquisition(before_replace=crash)
        self.assertEqual(self.state()["rows"], 1)
        self.assertGreater(self.archive.stat().st_size, len(self.member))
        self.assertEqual(len(self.ids()), 3)
        self.run_acquisition(replace=False)
        self.assert_advanced()

    def test_committed_prefix_is_skipped_when_same_job_is_refetched(self):
        def crash(source, destination):
            state = json.loads(source.read_text())
            if state["batch"] == 27:
                raise RuntimeError("interrupted batch completion")
        with self.assertRaises(RuntimeError):
            self.run_acquisition(before_replace=crash)
        self.assertEqual(self.state()["rows"], 3)
        committed = self.archive.read_bytes()
        self.run_acquisition(replace=False)
        self.assert_advanced()
        self.assertEqual(self.archive.read_bytes(), committed)

    def test_index_commit_before_failed_append_is_repaired_on_resume(self):
        real_open = Path.open

        def interrupted_open(path, mode="r", *args, **kwargs):
            if path == self.archive and mode == "ab":
                raise RuntimeError("interrupted archive append")
            return real_open(path, mode, *args, **kwargs)

        with patch.object(Path, "open", interrupted_open):
            with self.assertRaisesRegex(RuntimeError, "archive append"):
                self.run_acquisition()
        self.assert_initial_commit()
        self.assertEqual(len(self.ids()), 3)
        self.run_acquisition(replace=False)
        self.assert_advanced()

    def test_byte_cap_stops_before_index_or_archive_append(self):
        self.initial["max_bytes"] = len(self.member)
        self.write_state(self.initial)
        with self.assertRaisesRegex(RuntimeError, "byte cap"):
            self.run_acquisition()
        self.assert_initial_commit()
        self.assertEqual(self.ids(), [(1, 1)])

    def test_duplicate_id_is_rejected_before_archive_append(self):
        self.mode = "duplicate"
        with self.assertRaises(sqlite3.IntegrityError):
            self.run_acquisition()
        self.assert_initial_commit()
        self.assertEqual(self.ids(), [(1, 1)])

    def test_short_result_never_declares_completion(self):
        self.mode = "short"
        with self.assertRaisesRegex(RuntimeError, "Short authenticated result"):
            self.run_acquisition()
        self.assertFalse(self.state()["done"])
        self.assertEqual(self.state()["job"], URL + "new")

    def test_overflow_rejected_before_unexpected_rows_are_committed(self):
        self.mode = "overflow"
        with self.assertRaisesRegex(RuntimeError, "overflow"):
            self.run_acquisition()
        self.assert_initial_commit()

    def test_expired_download_session_retains_saved_job_and_commit(self):
        self.mode = "expired-download"
        with self.assertRaises(RuntimeError):
            self.run_acquisition()
        self.assert_initial_commit()
        self.assertEqual(self.state()["job"], URL + "new")
        self.assertFalse(any("--request" in c for c in self.calls))

    def test_cleanup_failure_keeps_next_checkpoint_and_never_retries_delete(self):
        self.mode = "delete-failure"
        with self.assertRaises(RuntimeError):
            self.run_acquisition()
        self.assert_advanced()
        self.assertIsNone(self.state()["job"])
        self.assertEqual(sum("--request" in c for c in self.calls), 1)

    def test_truncated_archive_is_not_padded_to_checkpoint_size(self):
        self.archive.write_bytes(self.member[:-1])
        with self.assertRaisesRegex(RuntimeError, "shorter"):
            self.run_acquisition()
        self.assertEqual(self.archive.stat().st_size, len(self.member) - 1)
        self.assertFalse(any("--dump-header" in c for c in self.calls))

    def test_cookie_jar_permissions_fail_before_any_request(self):
        self.jar.chmod(0o644)
        with contextlib.redirect_stderr(io.StringIO()), self.assertRaises(SystemExit):
            self.run_acquisition()
        self.assertEqual(self.calls, [])
        self.assertEqual(self.state(), self.initial)


if __name__ == "__main__":
    unittest.main()
