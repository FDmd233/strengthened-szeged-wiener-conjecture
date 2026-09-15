# Repository Publication and Integrity Record

**Repository:** `FDmd233/strengthened-szeged-wiener-conjecture`

**Reference package commit:** `f4dd375fbedc5153cc7aa13c8102f1c0f4eb3056`

**Package timestamp (UTC):** `2026-09-08T17:32:11Z`

**Verification workflow run:** `34257517977`

The commit above contains the manuscript source, compiled PDF, unpacked exact finite certificate, fixed Graph Atlas input snapshots, and packaged certificate archive.

The verification workflow uses NetworkX 3.6.1 to regenerate the fixed Atlas snapshots when needed, checks their SHA256 digests, compiles and executes the C++17 certificate, and compiles the manuscript with XeLaTeX.

The exact certificate reproduces the critical finite outputs, including

```text
F8 97
H9 681
G10 300311 min 20 bad 0
L5=10 pieces 12
L6=8 pieces 209
L7=11 pieces 3258
L8=14 pieces 48865
order8 nonspecial eta<=14 labelled_ext 65 L9_from_low=17
n10 105 min 20 bad 0
n11 160 min 23 bad 0
certificate verification: PASS
```

## Status note

This record documents public availability, reproducibility, and file integrity. It does **not** constitute peer review or independent mathematical endorsement. The manuscript is presented as a claimed proof of Conjecture 5 of Bonamy, Knor, Luzar, Pinlou and Skrekovski (2017); at the time of this record, MathDB still lists the problem as open and independent verification has been requested.
