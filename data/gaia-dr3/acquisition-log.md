# Gaia acquisition execution log

Production selection: Gaia DR3; all nine selected fields present;
ascending G magnitude, then source_id. No extra scientific cuts.

## Query: million.adql

```sql
SELECT TOP 1048576 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

## Submission: initial sandbox DNS failure, approved retry returned HTTP 303.

```sh
curl --fail --silent --show-error --max-time 40 --dump-header /tmp/msc-gaia-acquisition/submit.headers --output /tmp/msc-gaia-acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1048576 --data-urlencode QUERY@/tmp/msc-gaia-acquisition/million.adql https://gea.esac.esa.int/tap-server/tap/async
```

## Start request: exit 0.

```sh
curl --fail --silent --show-error --max-time 40 --output /tmp/msc-gaia-acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/2da48eba-ad53-11f1-b67b-bc97e148b76b-O/phase
```

## Status command executed repeatedly; last result COMPLETED. One sandbox DNS failure was retried with approval.

```sh
curl --fail --silent --show-error --max-time 40 --output /tmp/msc-gaia-acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2da48eba-ad53-11f1-b67b-bc97e148b76b-O
```

## Download: exit 0; 147,508,313 bytes; 1,048,576 validated rows.

```sh
curl --fail --silent --show-error --location --max-time 600 --output /tmp/msc-gaia-acquisition/first-result https://gea.esac.esa.int/tap-server/tap/async/2da48eba-ad53-11f1-b67b-bc97e148b76b-O/results/result
```

The previously proposed fetch-million.py helper was rejected before execution.
It did not run. The direct commands above were subsequently approved.
# Gaia DR3 acquisition log

All nine fields required. No added quality cuts.
Order: ascending G magnitude, then source_id. Final gzip size cap: 10000000000 bytes. Gzip members contain up to 4096 CSV rows; stop before a member would exceed the cap. This is not the complete Gaia catalogue.

## Invocation

```sh
python3 -B /home/amade/projects/portfolio/msc/tools/acquire_gaia.py --root /home/amade/projects/portfolio/msc/data/gaia-dr3 --max-bytes 10000000000 --initial-job https://gea.esac.esa.int/tap-server/tap/async/2da48eba-ad53-11f1-b67b-bc97e148b76b-O --initial-query /tmp/msc-gaia-acquisition/million.adql --initial-result /tmp/msc-gaia-acquisition/first-result
```
## Previously submitted initial query

```sql
SELECT TOP 1048576 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-10T20:16:37.830235+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2da48eba-ad53-11f1-b67b-bc97e148b76b-O
```
Exit code: 0
Reused initial result: /tmp/msc-gaia-acquisition/first-result

Validated batch: 1048576 inspected rows. Total committed: 1048576; gzip bytes: 68641402.

## Batch 1 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 10.824318885803223 OR (phot_g_mean_mag = 10.824318885803223 AND source_id > 1373328510295312768))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-10T20:17:15.479754+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00001.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-10T20:17:15.709497+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/9d802a62-ad54-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-10T20:17:15.965055+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d802a62-ad54-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:17:46.176896+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d802a62-ad54-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:18:16.408593+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d802a62-ad54-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:18:46.624642+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d802a62-ad54-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:19:16.864777+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d802a62-ad54-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:19:47.064045+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d802a62-ad54-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:20:17.267208+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d802a62-ad54-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:20:47.468449+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d802a62-ad54-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:20:47.838506+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/9d802a62-ad54-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 4048576; gzip bytes: 263468716.

## Batch 2 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 12.317307472229004 OR (phot_g_mean_mag = 12.317307472229004 AND source_id > 5974612354155283712))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-10T20:24:43.905996+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00002.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-10T20:24:45.743267+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-10T20:24:46.217073+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:25:16.445363+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:25:46.930579+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:26:17.136443+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:26:47.576098+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:27:17.760114+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:27:27.571730+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:27:47.951845+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:27:57.772425+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:28:18.141695+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:28:27.970318+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:28:48.350267+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:28:58.176299+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:29:18.565405+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:29:28.426779+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:29:48.792177+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:29:58.618999+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:30:19.041752+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:30:28.805242+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:30:49.244155+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:30:49.517985+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O/results/result
```

### 2026-09-10T20:30:59.066814+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:30:59.303367+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/a8f1e606-ad55-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 7048576; gzip bytes: 457490999.

## Batch 3 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 12.958824157714844 OR (phot_g_mean_mag = 12.958824157714844 AND source_id > 4071259423267570560))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-10T20:34:51.981970+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00003.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-10T20:34:52.447746+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/1345f39c-ad57-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-10T20:34:52.781700+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1345f39c-ad57-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:35:23.010062+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1345f39c-ad57-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:35:51.986692+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1345f39c-ad57-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:35:53.372947+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1345f39c-ad57-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:36:22.164889+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1345f39c-ad57-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-10T20:36:23.601677+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1345f39c-ad57-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:31:45.570548+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1345f39c-ad57-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:31:45.855081+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/1345f39c-ad57-11f1-b67b-bc97e148b76b-O/results/result
```

### 2026-09-11T06:33:36.980467+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1345f39c-ad57-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:33:37.227848+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/1345f39c-ad57-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 10048576; gzip bytes: 651102690.

## Batch 4 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 13.381890296936035 OR (phot_g_mean_mag = 13.381890296936035 AND source_id > 5972742948203122944))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T06:37:36.148603+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00004.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T06:37:36.402361+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/46c6c39a-adab-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T06:37:36.658917+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/46c6c39a-adab-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:38:06.855229+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/46c6c39a-adab-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:38:37.075571+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/46c6c39a-adab-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:39:07.279483+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/46c6c39a-adab-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:39:37.500873+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/46c6c39a-adab-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:39:37.758235+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/46c6c39a-adab-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 13048576; gzip bytes: 844404168.

## Batch 5 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 13.698129653930664 OR (phot_g_mean_mag = 13.698129653930664 AND source_id > 3393587015010544384))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T06:44:10.995399+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00005.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T06:44:11.270204+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/3222b238-adac-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T06:44:11.505987+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3222b238-adac-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:44:41.704450+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3222b238-adac-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:45:11.893721+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3222b238-adac-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:45:42.092035+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3222b238-adac-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:46:12.283411+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3222b238-adac-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:46:12.543418+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/3222b238-adac-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 16048576; gzip bytes: 1037476346.

## Batch 6 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 13.952475547790527 OR (phot_g_mean_mag = 13.952475547790527 AND source_id > 4117016836605643392))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T06:50:12.053755+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00006.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T06:50:12.316939+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/0957c888-adad-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T06:50:12.580417+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/0957c888-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:50:42.785714+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/0957c888-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:51:12.974036+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/0957c888-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:51:43.161502+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/0957c888-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:52:13.345555+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/0957c888-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:52:43.567409+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/0957c888-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:52:44.206864+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/0957c888-adad-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 19048576; gzip bytes: 1230356081.

## Batch 7 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 14.165837287902832 OR (phot_g_mean_mag = 14.165837287902832 AND source_id > 4040601057703111040))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T06:56:45.115391+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00007.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T06:56:45.864779+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T06:56:46.181659+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:57:16.397371+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:57:46.603946+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:58:17.099053+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:58:47.361718+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:59:17.925224+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T06:59:48.238692+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:00:18.435863+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:00:48.852604+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:01:19.067211+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:01:49.306929+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:02:19.779231+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:02:50.044118+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:02:50.809649+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/f3d0aa31-adad-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 22048576; gzip bytes: 1423067314.

## Batch 8 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 14.349931716918945 OR (phot_g_mean_mag = 14.349931716918945 AND source_id > 6902629584229197056))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T07:06:56.566248+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00008.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T07:06:57.378176+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T07:06:57.993780+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:07:28.402182+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:07:58.627365+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:08:29.022116+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:08:59.747648+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:09:30.000057+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:10:00.272867+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:10:30.682396+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:11:00.914272+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:11:31.304018+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:12:02.114102+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:12:32.541066+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:13:03.058778+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:13:33.983733+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:14:04.704142+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:14:34.978055+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:15:05.327417+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:15:35.841639+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:16:06.357837+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:16:36.871613+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:17:07.389186+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:17:08.321692+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/603e48c4-adaf-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 25048576; gzip bytes: 1615643044.

## Batch 9 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 14.512014389038086 OR (phot_g_mean_mag = 14.512014389038086 AND source_id > 5886833981552083840))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T07:21:12.155447+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00009.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T07:21:13.271742+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T07:21:15.607439+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:21:45.843459+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:22:16.093856+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:22:46.741182+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:23:17.460068+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:23:47.974379+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:24:18.490319+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:24:49.211634+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:25:20.955116+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:25:51.468658+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:26:22.393376+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:26:52.700957+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:27:23.216365+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:27:53.935983+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:28:24.451412+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:28:55.375587+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:29:25.891704+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:29:56.126246+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:30:26.713912+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:30:57.023992+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:30:59.176384+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/5e437c18-adb1-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 28048576; gzip bytes: 1808102032.

## Batch 10 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 14.657052993774414 OR (phot_g_mean_mag = 14.657052993774414 AND source_id > 239832696093103360))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T07:35:04.934366+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00010.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T07:35:05.544833+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T07:35:06.364059+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:35:37.184434+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:36:07.441771+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:36:37.696491+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:37:08.318316+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:37:38.545488+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:38:08.798925+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:38:39.033678+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:39:09.555561+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:39:40.071749+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:40:10.337068+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:40:40.895768+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:40:41.410245+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/4e745790-adb3-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 31048576; gzip bytes: 2000457858.

## Batch 11 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 14.788334846496582 OR (phot_g_mean_mag = 14.788334846496582 AND source_id > 1985605128026123392))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T07:44:48.870071+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00011.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T07:44:49.436404+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T07:44:50.257480+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:45:20.497970+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:45:50.768946+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:46:20.992053+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:46:51.268658+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:47:21.469808+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:47:51.682539+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:48:22.342301+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:48:52.854158+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:49:23.371307+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:49:53.889359+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:50:25.017880+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:50:55.322467+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:51:25.846377+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:51:27.589805+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/aa89d9c5-adb4-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 34048576; gzip bytes: 2192723627.

## Batch 12 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 14.908166885375977 OR (phot_g_mean_mag = 14.908166885375977 AND source_id > 3839541543891793792))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T07:55:34.582010+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00012.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T07:55:35.199285+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T07:55:35.703001+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:56:05.958131+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:56:36.533488+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:57:06.799416+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:57:37.359511+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:58:08.081533+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:58:38.598428+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:59:08.906264+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T07:59:39.421747+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:00:10.152482+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:00:40.657211+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:01:11.172506+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:01:41.426302+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:02:11.662613+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:02:41.882296+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:03:12.414784+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:03:12.932784+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/2b7a53b4-adb6-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 37048576; gzip bytes: 2384901975.

## Batch 13 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.018598556518555 OR (phot_g_mean_mag = 15.018598556518555 AND source_id > 5836284965337493888))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T08:07:19.867967+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00013.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T08:07:20.736429+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T08:07:21.351139+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:07:51.965937+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:08:22.688516+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:08:53.201269+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:09:23.718975+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:09:54.231801+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:10:25.156519+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:10:56.087239+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:11:26.359511+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:11:57.111512+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:12:27.623421+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:12:58.139836+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:13:29.064372+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:13:59.579339+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:14:30.095538+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:15:00.610711+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:15:31.123478+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:16:01.640774+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:16:32.203932+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:17:02.876700+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:17:33.395388+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:18:03.912400+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:18:34.430939+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:19:05.560863+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:19:36.075469+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:19:36.807456+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/cff46624-adb7-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 40048576; gzip bytes: 2577007453.

## Batch 14 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.121021270751953 OR (phot_g_mean_mag = 15.121021270751953 AND source_id > 3989002175249660800))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T08:23:45.202771+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00014.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T08:23:45.843569+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T08:23:46.662084+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:24:17.076700+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:24:47.589346+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:25:17.818301+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:25:48.260996+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:26:18.931816+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:26:49.652642+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:27:19.896851+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:27:50.479392+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:28:21.199940+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:28:51.719919+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:29:22.199315+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:29:52.747236+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:30:22.994767+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:30:53.574089+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:31:24.090389+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:31:55.079401+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:32:26.057929+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:32:56.521565+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:32:57.176861+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/1b2b43c4-adba-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 43048576; gzip bytes: 2769042161.

## Batch 15 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.216508865356445 OR (phot_g_mean_mag = 15.216508865356445 AND source_id > 2253214889446813184))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T08:37:05.785079+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00015.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T08:37:07.238744+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T08:37:08.672844+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:37:39.492844+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:38:10.016484+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:38:40.522190+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:39:11.244204+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:39:41.757174+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:40:12.274048+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:40:43.198940+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:41:13.714602+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:41:44.229479+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:42:15.565529+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:42:46.491022+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:43:17.415889+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:43:47.930154+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:44:18.444636+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:44:48.755643+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:45:19.682179+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:45:50.198901+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:46:21.325717+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:46:52.046880+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:47:22.561110+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:47:53.282585+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:48:23.528198+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:48:54.313281+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:49:24.827617+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:49:55.548592+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:50:26.270612+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:50:56.782985+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:51:27.504950+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:51:58.633666+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:52:28.900909+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:52:59.666578+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:53:30.386650+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:54:00.696508+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:54:31.625264+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:55:02.343858+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:55:32.859888+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:56:03.374353+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T08:56:03.891024+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/f87149b8-adbb-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 46048576; gzip bytes: 2961001498.

## Batch 16 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.305659294128418 OR (phot_g_mean_mag = 15.305659294128418 AND source_id > 6491769817238342144))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T09:00:13.119393+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00016.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T09:00:13.542522+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T09:00:13.805232+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:00:44.017597+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:01:14.239756+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:01:44.573486+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:02:14.823100+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:02:45.086949+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:03:15.509983+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:03:46.020767+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:04:16.536471+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:04:47.051989+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:05:17.772766+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:05:48.492279+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:06:19.214279+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:06:49.934308+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:07:20.448742+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:07:51.169106+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:08:21.686153+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:08:52.203516+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:08:53.329411+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/3325aad4-adbf-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 49048576; gzip bytes: 3152900144.

## Batch 17 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.389518737792969 OR (phot_g_mean_mag = 15.389518737792969 AND source_id > 5514012540849500032))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T09:13:01.489904+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00017.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T09:13:01.766740+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T09:13:02.776975+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:13:33.595314+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:14:04.024915+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:14:34.831238+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:15:05.755351+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:15:36.477852+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:16:07.196909+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:16:37.710985+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:17:08.432008+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:17:38.947082+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:18:10.076901+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:18:40.365234+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:19:10.903398+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:19:41.420041+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:20:11.932881+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:20:42.449248+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:21:12.963999+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:21:43.478573+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:22:13.995804+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:22:44.511187+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:23:15.229138+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:23:45.745633+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:24:16.267311+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:24:46.980958+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:25:17.907987+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:25:48.832668+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:26:19.704508+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:26:50.273328+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:27:20.995386+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:27:51.918647+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:28:22.437081+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:28:23.359524+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/fd1d60e4-adc0-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 52048576; gzip bytes: 3344745359.

## Batch 18 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.4686918258667 OR (phot_g_mean_mag = 15.4686918258667 AND source_id > 4205026493255430784))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T09:32:31.567801+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00018.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T09:32:32.193738+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T09:32:33.218887+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:33:04.446928+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:33:35.369430+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:34:06.091074+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:34:36.335098+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:35:06.917236+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:35:37.433399+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:36:08.085508+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:36:38.669771+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:37:09.388701+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:37:40.107109+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:38:10.624540+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:38:41.345052+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:39:12.065404+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:39:42.791171+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:40:13.709600+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:40:44.225790+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:41:14.580798+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:41:45.255896+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:42:15.976330+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:42:47.107973+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:43:17.620459+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:43:48.343015+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:44:19.062301+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:44:49.989096+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:45:21.116155+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:45:51.846104+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:46:22.558035+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:46:53.481109+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:47:24.213848+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:47:54.804815+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:48:25.233374+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:48:55.748511+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:49:26.790638+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:49:57.393956+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:50:28.318827+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:50:58.833785+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:51:29.759976+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:52:00.684168+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:52:31.609149+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:53:02.328967+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:53:33.064559+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:54:03.769139+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:54:34.695417+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:55:05.415488+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:55:36.544101+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:56:07.264807+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:56:37.991163+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:57:08.502340+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:57:39.425653+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:58:10.352167+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:58:40.868969+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:59:11.382707+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T09:59:42.922333+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:00:13.855114+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:00:44.774529+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:01:17.130647+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:01:47.849133+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:02:18.571477+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:02:49.494980+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:03:20.214900+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:03:51.104412+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:03:51.861545+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/b69eaa03-adc3-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 55048576; gzip bytes: 3536539174.

## Batch 19 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.543628692626953 OR (phot_g_mean_mag = 15.543628692626953 AND source_id > 6650266033294411392))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T10:08:02.039346+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00019.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T10:08:04.662999+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T10:08:05.609107+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:08:36.224319+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:09:06.738303+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:09:37.253075+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:10:07.564925+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:10:37.817295+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:11:08.389738+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:11:39.323121+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:12:10.036099+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:12:40.552005+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:13:10.860368+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:13:42.196785+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:14:13.122177+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:14:43.549167+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:15:14.152035+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:15:44.877073+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:16:15.386155+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:16:45.904754+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:17:16.215465+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:17:46.933533+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:18:17.653812+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:18:48.376125+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:19:18.888955+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:19:49.432548+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:20:19.920786+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:20:50.435153+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:21:21.160876+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:21:51.876998+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:22:22.127012+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:22:52.693573+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:23:22.935113+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:23:53.528561+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:24:24.043442+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:24:54.763700+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:24:55.898367+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/ad1d76c9-adc8-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 58048576; gzip bytes: 3728287203.

## Batch 20 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.614777565002441 OR (phot_g_mean_mag = 15.614777565002441 AND source_id > 281148151177348224))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T10:29:05.176418+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00020.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T10:29:05.953574+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T10:29:06.981354+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:29:37.593931+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:30:08.076578+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:30:38.625676+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:31:11.188857+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:31:41.702634+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:32:12.014360+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:32:42.528967+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:33:13.046308+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:33:43.278036+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:34:13.781689+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:34:44.591675+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:35:15.310975+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:35:47.264206+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:36:17.979813+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:36:48.703989+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:37:19.420597+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:37:50.145608+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:38:20.870812+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:38:51.785545+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:39:22.096841+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:39:52.669922+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:40:23.447123+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:40:54.256452+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:41:24.983329+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:41:56.107621+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:42:26.826911+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:42:57.753028+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:43:28.676715+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:43:59.398052+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:44:29.920523+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:45:00.838992+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:45:31.558695+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:46:02.279871+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:46:32.999957+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:47:03.719820+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:47:34.440207+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:48:04.954345+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:48:35.878738+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:49:06.599007+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:49:07.524918+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/9d74b1ad-adcb-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 61048576; gzip bytes: 3919992906.

## Batch 21 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.682665824890137 OR (phot_g_mean_mag = 15.682665824890137 AND source_id > 6010386549821879424))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T10:53:26.098582+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00021.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T10:53:26.375992+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T10:53:27.007223+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:53:58.032384+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:54:28.553296+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:54:59.063175+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:55:29.302188+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:55:59.890112+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:56:30.403784+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:57:01.537581+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:57:32.254768+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:58:03.178494+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:58:33.697607+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:59:04.000367+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T10:59:34.520575+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:00:04.736023+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:00:35.140588+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:01:05.450683+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:01:36.584191+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:02:07.302433+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:02:38.229739+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:03:08.467800+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:03:40.282635+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:04:11.215550+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:04:41.604494+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:05:12.238106+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:05:42.753792+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:06:13.268449+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:06:43.783925+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:07:14.252962+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:07:14.815086+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/040ffb97-adcf-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 64048576; gzip bytes: 4111661704.

## Batch 22 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.747464179992676 OR (phot_g_mean_mag = 15.747464179992676 AND source_id > 3068192183770377216))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T11:11:24.565700+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00022.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T11:11:25.490050+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T11:11:27.538862+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:11:58.362483+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:12:29.488927+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:13:00.208631+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:13:30.929323+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:14:01.650742+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:14:32.166056+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:15:02.412952+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:15:32.664322+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:16:03.303040+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:16:34.020461+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:17:04.537059+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:17:35.061823+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:18:05.362628+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:18:35.878070+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:19:06.598368+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:19:37.115192+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:20:07.830478+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:20:38.350769+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:21:09.275318+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:21:39.794466+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:22:10.715904+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:22:41.641512+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:22:42.155304+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/8739fee1-add1-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 67048576; gzip bytes: 4303302310.

## Batch 23 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.809562683105469 OR (phot_g_mean_mag = 15.809562683105469 AND source_id > 5981130572630388352))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T11:26:46.904732+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00023.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T11:26:47.185562+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T11:26:47.516555+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:27:18.121668+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:27:48.841475+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:28:19.072804+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:28:49.312463+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:29:19.980192+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:29:50.289163+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:30:20.805037+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:30:51.035810+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:31:21.631932+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:31:22.350104+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/aca3e96b-add3-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 70048576; gzip bytes: 4494912673.

## Batch 24 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.86918830871582 OR (phot_g_mean_mag = 15.86918830871582 AND source_id > 6851658424192198656))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T11:35:33.732143+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00024.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T11:35:34.255583+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T11:35:34.870556+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:36:05.488500+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:36:36.205081+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:37:06.459843+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:37:37.441500+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:38:07.957400+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:38:38.470715+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:39:09.085862+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:39:39.917467+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:40:10.837349+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:40:41.556376+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:41:12.275762+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:41:42.749969+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:42:13.308816+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:42:43.504180+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:43:14.544419+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:43:46.083229+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:43:46.803863+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/e6ad75f2-add4-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 73048576; gzip bytes: 4686484804.

## Batch 25 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.926590919494629 OR (phot_g_mean_mag = 15.926590919494629 AND source_id > 5299276683011419136))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T11:48:00.056536+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00025.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T11:48:00.963944+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T11:48:02.601007+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:48:33.420976+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:49:04.140418+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:49:34.861905+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:50:05.788613+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:50:36.711289+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:51:07.433978+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:51:38.151083+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:52:09.281691+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:52:40.413099+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:53:11.131910+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:53:41.852862+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:54:12.573375+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:54:43.703923+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:55:14.422335+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:55:46.166194+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:56:17.296956+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:56:48.249937+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:57:18.941687+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:57:49.662362+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:58:20.381279+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:58:51.510567+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:59:22.438326+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T11:59:53.157921+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:00:23.879166+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:00:54.803668+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:01:25.933389+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:01:56.652300+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:02:27.374039+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:02:58.298914+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:03:29.224654+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:03:59.943833+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:04:30.461661+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:05:00.975502+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:05:31.694678+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:06:02.414771+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:06:32.669686+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:07:03.447404+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:07:34.371793+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:08:05.092454+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:08:35.811817+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:09:06.533960+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:09:37.252846+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:10:07.972727+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:10:39.309508+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:11:10.232365+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:11:41.157245+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:12:12.083318+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:12:43.006660+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:13:15.572134+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:13:46.497900+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:14:17.832310+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:14:48.551010+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:15:19.475884+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:15:50.401456+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:16:21.941420+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:16:52.663766+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:17:23.380705+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:17:54.307768+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:18:25.031365+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:18:55.745900+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:19:26.470521+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:19:57.598174+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:20:28.329568+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:21:00.059934+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:21:30.984756+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:22:01.755966+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:22:32.425458+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:23:03.351849+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:23:34.684803+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:24:05.609816+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:24:36.330163+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:25:07.050847+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:25:37.770776+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:26:08.492454+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:26:40.235073+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:27:10.956154+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:27:41.675589+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:28:12.395427+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:28:43.527620+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:29:14.450819+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:29:45.171653+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:30:15.687915+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:30:47.433107+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:31:18.149497+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:31:49.691502+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:32:20.410956+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:32:51.132114+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:33:22.056453+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:33:52.979844+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:34:23.919907+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:34:54.627501+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:35:25.141755+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:35:55.861200+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:36:26.581847+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:36:57.300814+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:37:28.023558+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:37:59.155690+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:38:30.285501+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:39:01.002429+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:39:31.927610+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:40:03.264501+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:40:33.982745+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:41:04.703456+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:41:07.061861+00:00

```sh
curl --fail --silent --show-error --location --max-time 1800 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch.csv https://gea.esac.esa.int/tap-server/tap/async/a3bfca5e-add6-11f1-b67b-bc97e148b76b-O/results/result
```
Exit code: 0

Validated batch: 3000000 inspected rows. Total committed: 76048576; gzip bytes: 4878012154.

## Batch 26 query

```sql
SELECT TOP 3000000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T12:45:25.256640+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=3000000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 0

### 2026-09-11T12:45:26.339929+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/start.body --data-urlencode PHASE=RUN https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O/phase
```
Exit code: 0

### 2026-09-11T12:45:29.821354+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:46:00.452418+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:46:30.950336+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:47:01.678814+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:47:32.393052+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:48:03.726007+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:48:34.649913+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:49:05.370078+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:49:36.150037+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:50:07.021220+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:50:37.942489+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:51:09.070826+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:51:40.200592+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:52:10.920103+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:52:41.641007+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:53:12.360616+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:53:43.080455+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:54:14.007026+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:54:44.931478+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:55:15.652100+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:55:46.372638+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:56:17.296473+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:56:48.221762+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:57:19.356860+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:57:50.073695+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:58:21.201032+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:58:51.921087+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:59:22.645143+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T12:59:53.361839+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:00:24.082096+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:00:54.802627+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:01:25.728309+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:01:56.242458+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:02:26.961927+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:02:57.682627+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:03:28.608657+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:03:59.328653+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:04:31.689711+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:05:02.817154+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:05:33.744626+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:06:04.872975+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:06:35.799239+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:07:06.525185+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:07:37.355876+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:08:08.217122+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:08:38.886468+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:09:09.806486+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:09:40.733385+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:10:11.656946+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:10:42.582180+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:11:13.919803+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:11:44.637637+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:12:15.356962+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:12:46.283305+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:13:17.414849+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:13:48.336960+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:14:19.058919+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:14:49.778039+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:15:21.523462+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:15:52.653378+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:16:23.786330+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:16:54.503169+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:17:25.427032+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:17:55.943371+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:18:26.867852+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:18:57.588233+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:19:28.512127+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:19:59.852238+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:20:30.158432+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:21:02.928857+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:21:33.456107+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:22:03.834028+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:22:34.678790+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:23:05.396832+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:23:36.936775+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:24:07.657970+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:24:38.381174+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:25:09.507494+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:25:40.023422+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:26:10.742700+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:26:41.872654+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:27:12.798657+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:27:43.722814+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:28:14.648313+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:28:45.572083+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:29:16.497459+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:29:47.223026+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:30:18.142529+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:30:49.067788+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:31:21.427228+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:31:52.350057+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:32:23.327801+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:32:54.318689+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:33:25.330619+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:33:56.258507+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:34:27.181120+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:34:58.105607+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:35:29.645159+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:36:01.592929+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:36:32.519708+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:37:03.444310+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:37:34.372309+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:38:05.294268+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:38:38.061992+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:39:09.849516+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:39:41.141907+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:40:12.270474+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:40:43.199191+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:41:14.128093+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:41:45.045922+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:42:15.973235+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:42:46.895388+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:43:19.048956+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:43:50.183983+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:44:21.104587+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:44:52.030337+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:45:22.749489+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:45:54.700149+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:46:25.624403+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:46:57.162045+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:47:28.293607+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:48:00.036393+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:48:30.757490+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:49:01.682086+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:49:32.403628+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:50:03.739350+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:50:34.662683+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:51:05.390396+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:51:36.512867+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:52:07.436610+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:52:38.158246+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:53:09.493343+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:53:40.416433+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:54:11.575120+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:54:42.881968+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:55:13.807394+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:55:44.732283+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:56:15.861346+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:56:46.787072+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:57:18.736420+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:57:49.661467+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:58:20.857644+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:58:51.715169+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:59:22.641806+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T13:59:53.575758+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:00:25.514217+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:00:56.440199+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:01:27.576532+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:01:58.289489+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:02:29.214697+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:03:00.345423+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:03:31.071314+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:04:01.991683+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:04:32.710293+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:05:04.043566+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:05:35.173391+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:06:06.303692+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:06:37.229017+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:07:08.155759+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:07:38.923178+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:08:10.006501+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:08:41.341189+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:09:12.269424+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:09:43.188050+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:10:13.907757+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:10:44.834616+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:11:16.781309+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:11:48.528020+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:12:19.248198+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:12:50.182198+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:13:21.096236+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:13:52.023629+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:14:22.947571+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:14:53.872731+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:15:24.797854+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:15:55.724344+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:16:26.648374+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:16:57.572543+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:17:28.496503+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:17:59.216094+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:18:29.938496+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:19:00.863613+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:19:31.787311+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:20:03.531991+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:20:34.456720+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:21:06.327535+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:21:37.126572+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:22:08.256052+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:22:39.104163+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:23:10.310791+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:23:41.441143+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:24:12.365045+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:24:43.290063+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:25:14.215428+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:25:45.962320+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:26:16.678599+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:26:48.021737+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:27:18.735282+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:27:49.456639+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:28:20.379699+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:28:51.305606+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:29:22.232788+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:29:53.177438+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:30:24.082958+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:30:55.005644+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:31:26.359090+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:31:57.470199+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:32:28.395019+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:32:59.319571+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:33:30.860404+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:34:01.579146+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:34:32.503064+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:35:03.429125+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:35:34.560000+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:36:05.486018+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:36:37.228296+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:37:08.361345+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:37:39.282585+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:38:10.208984+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:38:41.134488+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:39:12.266869+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:39:44.007073+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:40:14.937432+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:40:45.858395+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:41:16.988975+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:41:48.527234+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:42:20.479401+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:42:51.405473+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:43:22.735826+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:43:53.660945+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:44:24.585569+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:44:55.510668+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:45:26.435172+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:45:58.384283+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:46:29.515842+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:47:00.440008+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:47:31.364321+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:48:02.496470+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:48:33.420266+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:49:04.343700+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:49:35.270142+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:50:07.422341+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:50:38.561002+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:51:10.119553+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:51:42.450120+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:52:15.219159+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:52:47.170682+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T14:52:49.116871+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/error.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O/error
```
Exit code: 0

### 2026-09-11T17:04:22.921925+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/job.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O
```
Exit code: 0

### 2026-09-11T17:04:38.832629+00:00

```sh
curl --fail --silent --show-error --location --max-time 60 --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/error.xml https://gea.esac.esa.int/tap-server/tap/async/a94208b0-adde-11f1-b67b-bc97e148b76b-O/error
```
Exit code: 0

ESA timed out batch 26 at 3000000 rows; retrying from the unchanged cursor with 1500000 rows.

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T17:04:45.148543+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 28
```text
curl: (28) Operation timed out after 60001 milliseconds with 0 bytes received

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:21:24.036344+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 28
```text
curl: (28) Operation timed out after 60000 milliseconds with 0 bytes received

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:22:57.480961+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 28
```text
curl: (28) Operation timed out after 60001 milliseconds with 0 bytes received

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:24:31.126733+00:00

```sh
curl --fail --silent --show-error --max-time 60 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:26:41.511639+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 28
```text
curl: (28) Operation timed out after 300000 milliseconds with 0 bytes received

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:32:14.901349+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:35:49.177595+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:39:23.639605+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:42:58.202195+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:46:32.784913+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:50:07.743963+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:53:42.173333+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T18:57:16.935044+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T19:00:51.684206+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T19:04:26.412603+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T19:08:01.432824+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T19:11:43.188817+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T19:15:21.008845+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T19:18:56.654678+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 28
```text
curl: (28) Operation timed out after 300000 milliseconds with 0 bytes received

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T19:24:30.515870+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-11T19:28:12.120924+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T12:54:25.818916+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:04:46.506908+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:08:50.966301+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:12:55.842676+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:17:01.197140+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:21:05.741337+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:25:10.966904+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:29:18.967083+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:33:23.250561+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:37:28.314951+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:41:32.513682+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:45:37.543658+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:49:42.778381+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:53:47.827481+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T13:57:52.230601+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:01:57.571389+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:06:02.811051+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:10:10.369580+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:14:16.144896+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:18:23.881281+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:22:28.503196+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:26:33.021992+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:30:37.491311+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:34:42.460814+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:38:48.603260+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:42:54.418892+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:46:59.813198+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:51:04.404649+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:55:09.520266+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T14:59:15.932504+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:03:24.929090+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:07:38.860298+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:11:44.999247+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:16:10.495398+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 28
```text
curl: (28) Operation timed out after 300000 milliseconds with 0 bytes received

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:22:15.722322+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:26:20.422767+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:30:26.366767+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:34:31.506079+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:38:36.462032+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:42:41.923301+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:46:46.427941+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:50:50.754177+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:54:55.430956+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T15:59:00.284540+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:03:05.555838+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:07:10.203660+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:11:15.489312+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:15:19.764357+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:19:24.357883+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:23:28.915596+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:27:33.838181+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:31:38.284185+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:35:43.438797+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:39:48.461018+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:43:54.051298+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:47:58.851815+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:52:03.243722+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T16:56:08.793017+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T17:00:13.356726+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 22
```text
curl: (22) The requested URL returned error: 500

```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 1500000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T17:04:17.720883+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=1500000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```

## Invocation

```sh
./tools/download_gaia_catalogue
```

## Batch 26 query

```sql
SELECT TOP 750000 source_id, ref_epoch, ra, dec, pmra, pmdec, parallax, phot_g_mean_mag, bp_rp
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
  AND (phot_g_mean_mag > 15.981961250305176 OR (phot_g_mean_mag = 15.981961250305176 AND source_id > 5938936058012995200))
ORDER BY phot_g_mean_mag ASC, source_id ASC
```

### 2026-09-12T19:26:30.979162+00:00

```sh
curl --fail --silent --show-error --max-time 300 --dump-header /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.headers --output /home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/submit.body --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=csv --data-urlencode MAXREC=750000 --data-urlencode QUERY@/home/amade/projects/portfolio/msc/data/gaia-dr3/.acquisition/batch-00026.adql https://gea.esac.esa.int/tap-server/tap/async
```
Exit code: 28
```text
curl: (28) Operation timed out after 300000 milliseconds with 0 bytes received

```

## 2026-09-12 recovery outcome

Batch size was reduced from 1,500,000 to 750,000 rows.
`msc-gaia-acquisition-750k.service` ran once without automatic restart.
ESA async submission timed out after 300000 ms with zero bytes received
(curl exit 28). No job was recorded: `job` is null and `job_started` is false.
The checkpoint remains 76,048,576 rows and 4,878,012,154 bytes, at batch 26
with `job_limit` 750000 and `done` false.
The service is stopped/failed; acquisition remains incomplete.
