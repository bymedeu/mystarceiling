# Hipparcos-2 acquisition

```sh
/home/amade/.pyenv/versions/3.12.9/bin/python3 /home/amade/projects/portfolio/msc/tools/acquire_hipparcos2.py --root /home/amade/projects/portfolio/msc/data/hipparcos2 --gaia-root /home/amade/projects/portfolio/msc/data/gaia-dr3 --hp-limit 1.9425238
```

## 2026-09-12T16:28:59.761872+00:00

```sh
curl --fail --silent --show-error --location --max-time 120 --output /home/amade/projects/portfolio/msc/data/hipparcos2/runs/20260912T162859.761589Z/ReadMe https://cdsarc.cds.unistra.fr/ftp/I/311/ReadMe
```
Exit code: 0

```text

```

## 2026-09-12T16:28:59.984637+00:00

```sh
curl --fail --silent --show-error --location --max-time 120 --output /home/amade/projects/portfolio/msc/data/hipparcos2/runs/20260912T162859.761589Z/hip2.dat.gz https://cdsarc.cds.unistra.fr/ftp/I/311/hip2.dat.gz
```
Exit code: 0

```text

```

## 2026-09-12T16:29:06.399910+00:00

```sh
curl --fail --silent --show-error --location --max-time 120 --output /home/amade/projects/portfolio/msc/data/hipparcos2/runs/20260912T162859.761589Z/match-count.xml --get --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=votable --data-urlencode MAXREC=1000000 --data-urlencode 'QUERY=SELECT COUNT(*) AS match_count FROM gaiadr3.hipparcos2_best_neighbour AS x WHERE x.original_ext_source_id IN (7588,15863,21421,24436,24608,25336,25428,26311,26727,27989,28360,30324,30438,31681,32349,33579,36850,37279,37826,39953,45238,49669,60718,61084,62434,62956,65474,67301,68702,69673,71681,71683,80763,85927,86228,90185,91262,97649,100751,102098,109268,113368)' https://gaia.aip.de/tap/sync
```
Exit code: 0

```text

```

## 2026-09-12T16:29:08.316285+00:00

```sh
curl --fail --silent --show-error --location --max-time 120 --output /home/amade/projects/portfolio/msc/data/hipparcos2/runs/20260912T162859.761589Z/matches.xml --get --data-urlencode REQUEST=doQuery --data-urlencode LANG=ADQL --data-urlencode FORMAT=votable --data-urlencode MAXREC=1000000 --data-urlencode 'QUERY=SELECT x.original_ext_source_id, x.angular_distance, x.number_of_neighbours, x.xm_flag, g.source_id, g.ref_epoch, g.ra, g.dec, g.pmra, g.pmdec, g.parallax, g.phot_g_mean_mag, g.bp_rp FROM gaiadr3.hipparcos2_best_neighbour AS x LEFT OUTER JOIN gaiadr3.gaia_source AS g ON x.source_id = g.source_id WHERE x.original_ext_source_id IN (7588,15863,21421,24436,24608,25336,25428,26311,26727,27989,28360,30324,30438,31681,32349,33579,36850,37279,37826,39953,45238,49669,60718,61084,62434,62956,65474,67301,68702,69673,71681,71683,80763,85927,86228,90185,91262,97649,100751,102098,109268,113368) ORDER BY x.original_ext_source_id ASC, g.source_id ASC' https://gaia.aip.de/tap/sync
```
Exit code: 0

```text

```

## Validated outcome

```json
{
  "format_version": 1,
  "release": "Hipparcos-2 (van Leeuwen 2007), CDS I/311",
  "utc_started": "2026-09-12T16:28:59.761589+00:00",
  "utc_completed": "2026-09-12T16:29:15.189822+00:00",
  "script_sha256": "fc111db4033d3470354486feae35906e931524f88a799bb52bda60ed67e23ec1",
  "source_url": "https://cdsarc.cds.unistra.fr/ftp/I/311/",
  "selection": {
    "expression": "hp_mag < hp_limit",
    "hp_limit": 1.9425238,
    "band": "Hipparcos Hp",
    "required_native_columns": [
      "hip_id",
      "ref_epoch",
      "ra",
      "dec",
      "pmra",
      "pmdec",
      "parallax",
      "hp_mag",
      "b_v"
    ],
    "provisional": true,
    "equivalent_to_gaia_g_cut": false
  },
  "counts": {
    "source_rows": 117955,
    "complete_native_rows": 117955,
    "outside_hp_selection": 117913,
    "selected_rows": 42
  },
  "crossmatch_status_row_counts": {
    "no_published_best_neighbour": 40,
    "matched_not_locally_downloaded": 2
  },
  "crossmatch_endpoint": "https://gaia.aip.de/tap/sync",
  "crossmatch_table": "gaiadr3.hipparcos2_best_neighbour",
  "gaia_snapshot": {
    "kind": "incomplete_checkpoint",
    "path": "/home/amade/projects/portfolio/msc/data/gaia-dr3/gaia-dr3.csv.gz.partial",
    "checkpoint": {
      "max_bytes": 10000000000,
      "rows": 76048576,
      "bytes": 4878012154,
      "batch": 26,
      "cursor": [
        15.981961250305176,
        5938936058012995200
      ],
      "job": null,
      "job_started": false,
      "job_limit": 1500000,
      "done": false,
      "benchmarks": {
        "4096": {
          "gzip_prefix_bytes": 273074
        },
        "65536": {
          "gzip_prefix_bytes": 4341059
        },
        "262144": {
          "gzip_prefix_bytes": 17247588
        },
        "1048576": {
          "gzip_prefix_bytes": 68641402
        }
      },
      "rejected_rows": 0,
      "target_rows": 134217728
    },
    "checkpoint_sha256": "e79936711066053a136c31d113d97fe26be05ac47d3e8ac41b430ec5f2a02f7d",
    "membership_evidence": "read-only acquisition ID index, bounded by checkpoint row count"
  },
  "units": {
    "ra": "degrees ICRS",
    "dec": "degrees ICRS",
    "ref_epoch": "Julian year 1991.25",
    "pmra": "mas/yr, RA motion with cos(dec)",
    "pmdec": "mas/yr",
    "parallax": "mas",
    "hp_mag": "Hipparcos Hp mag",
    "b_v": "B minus V mag",
    "angular_distance": "arcsec"
  },
  "order": [
    "hp_mag ASC",
    "hip_id ASC"
  ],
  "limitations": [
    "Hp cutoff is not a Gaia G cutoff.",
    "No photometric conversion or position propagation was performed.",
    "No published best neighbour does not establish absence from Gaia.",
    "Local Gaia membership is a snapshot, not a final fusion decision.",
    "Native source flags, errors and other fields remain in hip2.dat.gz.",
    "Multiple links are retained; no ambiguous identity is silently resolved."
  ],
  "files": {
    "ReadMe": {
      "bytes": 10375,
      "sha256": "6c017925d658447d40983da3b459ba16bf9789c8f7573e263952c75692622499"
    },
    "gaia-crossmatch.csv.gz": {
      "bytes": 505,
      "sha256": "63e3a57795789d907a72d703e56e9fa5a0dfca063b41bc70eaf96d01c4eee0e3"
    },
    "hip2.dat.gz": {
      "bytes": 8977907,
      "sha256": "8e624f843d4254a9b7c2e8dda8e3158dbe825bf98f6bf3dbbae7f0d0b73d6858"
    },
    "hipparcos2.csv.gz": {
      "bytes": 1831,
      "sha256": "4fff5dcc909c54c668b91572fc03269597ffc0f4c84505aeded55ec6d831feea"
    },
    "match-count.xml": {
      "bytes": 1717,
      "sha256": "67cb2890dfe89096fb67be98f3d3fed4c8ee9a5941cbfd560c88e9e3c7b0d3a7"
    },
    "match-count.xml.adql": {
      "bytes": 366,
      "sha256": "fe52c9a64a4746f8e09e4850fa12cd77baaefec0a63b491cfd18e2b64ff0d534"
    },
    "matches.xml": {
      "bytes": 5491,
      "sha256": "9b601f9d3e230b985ab4eaea906fb66f5932a06966c17dff4078d5357f66b021"
    },
    "matches.xml.adql": {
      "bytes": 643,
      "sha256": "f19246d5841abcbe2dea82418c3a80fe32c228404963bbb7f7a6b65b6922d931"
    }
  }
}
```
