# Exact certificate for *The Strengthened Szeged--Wiener Conjecture*

Author: Zhijie He (何芝杰)

This archive verifies only the explicitly finite propositions in Section 5 of
the paper.  The structural reduction, the exceptional dominated-extension
bounds, the block assembly inequality, and the induction are proved in the
paper itself.  No numerical approximation is used anywhere in this archive.

## What is checked

1. **Small pieces.**  For piece orders 5, 6, 7, 8 the verifier checks every
   dominated extension of every non-special biconnected deletion block in the
   Read--Wilson Graph Atlas.  It obtains
   `L(5)=10, L(6)=8, L(7)=11, L(8)=14`.
   For order 9, Lemma 2.2 of the paper reduces the check to order-eight
   biconnected non-special deletion graphs with eta at most 14; every such
   graph is generated from a connected seven-vertex Atlas graph by restoring
   one deleted vertex.  The resulting minimum is `L(9)=17`.

2. **The two final block gluings.**  Only `(6,6)` at order 10 and `(6,7)` at
   order 11 survive the analytic deficit estimate.  The exact minima are 20
   and 23, respectively.

3. **Order-ten base.**  The proof reduces any hypothetical order-ten graph
   with eta below 20 to two successive dominated extensions of an order-eight
   biconnected graph with eta at most 15.  The verifier deliberately keeps
   repeated labelled occurrences.  It checks
   `97 -> 681 -> 300311` generated occurrences and obtains minimum eta 20,
   with no graph below 20.

The three exceptional graph families are recognized by degree sequences.  In
these three cases the relevant degree sequence uniquely determines the graph,
so no graph-isomorphism algorithm is used in the critical base check.

## Verification

A POSIX shell and a C++17 compiler are sufficient:

```sh
./verify.sh
```

The script

- checks the SHA256 hashes of the fixed input snapshots;
- compiles all C++ sources with warnings treated as errors;
- runs a self-test of the exact formula `eta(K_n^t)=2(t-1)(n-t-1)`;
- runs the three finite verifiers; and
- compares the resulting output byte-for-byte with `EXPECTED_RESULTS.txt`.

A successful run ends with

```text
certificate verification: PASS
```

## Input snapshots

The fixed snapshots are under `input/`.  Their expected hashes are recorded in
`SHA256SUMS.expected`.  They were exported from the Read--Wilson Graph Atlas
using NetworkX 3.6.1.  The optional script `export_atlas.py` regenerates them;
NetworkX is **not** required for verification once the snapshots are present.

The files are:

- `connected7_graph6.txt` -- the 853 connected seven-vertex Atlas graphs in
  graph6 format;
- `connected7_masks.txt` -- the same 853 graphs as adjacency bitmasks;
- `atlas_bases_4_7.txt` -- all biconnected Atlas representatives of orders
  4 through 7 as adjacency bitmasks.

## Source layout

- `src/exact_graph.hpp` -- direct integer definitions of Wiener index, Szeged
  index, eta, biconnectivity, exceptional-family recognition, and dominated
  extension;
- `src/piece_audit.cpp` -- Proposition 5.1;
- `src/glue_audit.cpp` -- Proposition 5.2;
- `src/base10_audit.cpp` -- Proposition 5.3;
- `src/self_test.cpp` -- formula sanity check;
- `verify.sh` -- one-command build and verification.
