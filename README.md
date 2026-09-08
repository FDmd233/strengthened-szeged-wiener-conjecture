# The Strengthened Szeged--Wiener Conjecture

This repository contains the manuscript **The Strengthened Szeged--Wiener Conjecture** by **Zhijie He** and its exact finite certificate.

The manuscript gives a claimed proof of Conjecture 5 of Bonamy, Knor, Luzar, Pinlou and Skrekovski (2017): if `G` is a 2-connected graph of order `n >= 10` and

\[
G \not\cong K_n,\qquad G \not\cong K_n^2,\qquad G \not\cong K_n^{n-2},
\]

then

\[
\operatorname{Sz}(G)-W(G)\ge 2n.
\]

The argument completes the dominated-vertex induction from the 2017 paper. Complete dominated deletions are handled by an exact formula for `K_n^t`; the two exceptional 2-connected deletions are treated by direct true-twin counts; and non-2-connected deletions are reduced to finitely many low-order block pieces.

> **Verification status.** The manuscript has been submitted to MathDB as a proposed solution. Independent verification has been requested; MathDB currently keeps the problem open. The repository is therefore presented as a reproducible claimed proof, not as a claim of peer-reviewed acceptance.

## Manuscript

- [PDF](Szeged_Wiener_2026-09-08.pdf)
- [LaTeX source](Szeged_Wiener_2026-09-08.tex)

## Exact finite certificate

The finite verification used in Section 5 is available both as an unpacked directory and as the original archive:

- [`certificate/`](certificate/) -- complete reproducible certificate
- [`Szeged_Wiener_Exact_Certificate_2026-09-08.zip`](Szeged_Wiener_Exact_Certificate_2026-09-08.zip) -- archive submitted with the manuscript

The certificate uses only integer arithmetic. Its critical order-ten check does not rely on graph-isomorphism deduplication or floating-point computation.

To run the complete certificate on a POSIX system with a C++17 compiler:

```sh
cd certificate
./verify.sh
```

A successful run ends with

```text
certificate verification: PASS
```

The expected output includes

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
```

GitHub Actions also runs the exact certificate automatically on pushes and pull requests.

## Build the manuscript

The manuscript is written for XeLaTeX. With `latexmk` and the required CJK fonts/packages installed, run

```sh
make paper
```

or compile directly with XeLaTeX.

## Integrity

SHA256 checksums for the manuscript, archive, and unpacked certificate files are recorded in [`SHA256SUMS`](SHA256SUMS). The certificate additionally contains its own fixed-input checksum file, [`certificate/SHA256SUMS.expected`](certificate/SHA256SUMS.expected).

## Problem and source

- MathDB problem: https://mathdb.com/p/332514/the-strengthened-szeged-wiener-lower-bound-for-2-connected-g
- M. Bonamy, M. Knor, B. Luzar, A. Pinlou and R. Skrekovski, *On the difference between the Szeged and the Wiener index*, Applied Mathematics and Computation **312** (2017), 202--213. Conjecture 5 is the strengthened bound addressed here.

## Author

Zhijie He
