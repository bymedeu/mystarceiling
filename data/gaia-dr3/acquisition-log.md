# Gaia DR3 acquisition record

Completed selected catalogue, not the full Gaia release. Source: ESA Gaia Archive,
`gaiadr3.gaia_source`; TAP endpoint: `https://gea.esac.esa.int/tap-server/tap/async`.
All nine fields below are mandatory/non-null; no additional scientific cuts.
Order: ascending G magnitude, then ascending source_id; smaller datasets are prefixes.

## Canonical selection and continuation

```sql
SELECT TOP {batch_limit} source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
FROM gaiadr3.gaia_source
WHERE source_id IS NOT NULL
  AND ref_epoch IS NOT NULL
  AND ra IS NOT NULL
  AND dec IS NOT NULL
  AND pmra IS NOT NULL
  AND pmdec IS NOT NULL
  AND parallax IS NOT NULL
  AND phot_g_mean_mag IS NOT NULL
  AND bp_rp IS NOT NULL
  AND (phot_g_mean_mag > {last_mag} OR (phot_g_mean_mag = {last_mag} AND source_id > {last_id}))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

Omit the final cursor predicate for the first batch. Use the last committed key;
recover G's exact float32 value before serializing it as a Python float literal,
and retain source_id as an integer. `batch_limit` is the smaller of the current
batch allowance and remaining target rows; TAP MAXREC equals it. Adaptive batch
reductions change transfer size, never selection or order.

## Reproduction

Downloader version: Git `9b10552d29e6e40d80355fa1b7d78cc0ecac425b`
(`tools/acquire_gaia.py`; wrapper `tools/download_gaia_catalogue`). From repo root:

```sh
./tools/download_gaia_catalogue
# Equivalent, including authenticated duration default:
python3 -B tools/acquire_gaia.py --root data/gaia-dr3 \
  --target-rows 134217728 --max-bytes 10000000000 --execution-duration 600
```

These are reproducible entry points, not a transcript of every historical run.
An authenticated session is supplied via `MSC_GAIA_COOKIE_JAR`, pointing to an
external private jar. The downloader does not log in or refresh the session.
Existing state resumes; a fresh root starts from zero. See
[recovery contracts](../../docs/gaia-recovery.md) for one-shot timeout replacement.
The archive is CSV in concatenated gzip members (one header, up to 4096 rows per
member), with a hard 10,000,000,000-byte compressed cap and unique-ID validation.

## Outcome and verification scope

- Completed UTC: `2026-10-18T18:20:08.887686+00:00`; stop: target row count.
- Rows: **134,217,728**; compressed bytes: **8,575,699,250** (about 7.987 GiB).
- Output: local `gaia-dr3.csv.gz`; final key: `(16.78596305847168, 5311740746825005568)`.
- Archive SHA256: `cf0db34640b44208036bdeb2660e0bf9213acf83d793503f9187d367b37764f2`.

The downloader checked per-batch schema, strict order and duplicate IDs, then
streamed the final gzip to EOF, counted rows and computed the archive/prefix
hashes. The local untracked `manifest.json` retains benchmark-prefix hashes.
Independent follow-up checked archive/manifest/checkpoint metadata agreement and
absence of partial archive/index; it did not independently decompress, recount
or rehash the full archive. See [canonical completion evidence](../../docs/gaia-datasets.md#verified-acquisition-evidence).

## Failure summary and remaining maintenance

Many repeated submission, transport/DNS and polling failures occurred. Bounded
retries/resume retained committed progress. Anonymous jobs repeatedly hit their
60-second execution quota; smaller batches did not resolve the blocked cursor.
Authenticated recovery verified a 600-second allowance, committed a checked
250,000-row batch and then completed the target through bounded continuation.
The advertised TAP maximum alone did not establish the anonymous allowance.

This record replaces the repetitive request/poll transcript with owner approval.
The Python downloader still appends verbose Markdown: changing that logger is
future maintenance, outside this documentation task. Acquisition is complete
and not running, so no active accumulation remains. Future request debugging
should use optional ignored runtime traces, with no credentials/response bodies
in public human documentation.
