# Bright-source regression fixtures

`hip2-bright.dat` contains seven unmodified 276-column rows from CDS I/311,
Hipparcos-2 (van Leeuwen 2007): HIP 14576, 24436, 24608, 25930, 27989, 32349,
58001. Retrieved 2026-10-28 from the official
[CfA VizieR mirror](https://vizier.cfa.harvard.edu/ftp/cats/I/311/hip2.dat.gz).
The full compressed source has SHA256
`8e624f843d4254a9b7c2e8dda8e3158dbe825bf98f6bf3dbbae7f0d0b73d6858`.
Its schema is retained with the acquisition run; these rows remain catalogue
data, not software relicensed under MSC's MIT licence. Acknowledge CDS/VizieR
and the original catalogue when reusing them.

`match-count-0000.xml`, `matches-0000.xml` and their exact `.adql` queries are
published Gaia DR3 Hipparcos-2 best-neighbour evidence from Gaia@AIP, retrieved
on the same date. Only Mintaka has a published link in this subset, to source
3220756843825383936, G=2.2487726, with missing pmra/pmdec/parallax. Neither a
positional query nor the nearby G=6.8278775 companion supplies that identity.

`prepared.csv.gz` is deterministic preparation of these seven rows using the
accepted Hp/B-V or Hp/V-I estimates and the inclusive estimated-G<=6 policy.
The Python regression recomputes it from the raw rows and published evidence;
the CLI integration test consumes it through the actual C++ build/binary path.
Algol, Mintaka and Phecda have estimated G=2.0742011124304, 2.185029 and
2.3868610055232: all fail the old <1.9425238 selection. The other four protect
previous bright-source coverage. Exact expected magnitudes test the formula,
not scientific precision: published fit scatter is 0.02417/0.08191 mag and
variable/multiple systems can differ more.

C++ position checks use independent first-order tangent propagation from the
native J1991.25 angles/proper motions to J2016. A 1 mas angular-coordinate
bound covers the neglected second-order terms for these seven rows (<0.7 mas,
dominated by Sirius). This tests pipeline/epoch errors, not external astrometric
accuracy. Native position and proper-motion rounding, missing radial velocity,
photometric variability and unresolved components retain the existing model limits.
