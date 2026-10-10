# Independent audit of the final dominated-vertex induction

Scope: this audit checks the logic *after* the source-paper lemmas
and the supplied finite certificate have been established.
It records exactly what is required to turn those local inputs
into the conjectured all-orders theorem. It is not a replacement for
the original graph enumeration or a journal referee's certification.

## Statement and hypotheses

For a simple 2-connected graph \(G\) of order \(n\ge10\), excluding
\(K_n,K_n^2,K_n^{n-2}\), the paper claims
\(\eta(G)=\operatorname{Sz}(G)-W(G)\ge2n\).
All "dominated pairs" \((u,v)\) are required to have **distinct**
\(u\ne v\) and \(N[u]\subseteq N[v]\). Without the distinctness
convention the quoted deletion lemma is false (because \(u=v\)
would qualify trivially).

The proof invokes three genuinely external results from
Bonamy--Knor--Lužar--Pinlou--Škrekovski (2017):
(1) the dominated-deletion improvement \(\eta(G)-\eta(G-u)\ge2\)
when \(G-u\) remains 2-connected and non-complete;
(2) the unique-cut-vertex block structure when *every*
dominated deletion fails 2-connectivity or is complete;
(3) \(\eta(G)\ge2|G|\) for a 2-connected non-complete
graph with no distinct dominated pair, except the quoted \(C_5\)
boundary. The special deletions \(K_{n-1}^2,K_{n-1}^{n-3}\)
are handled separately by the manuscript's exact true-twin
polynomial tables, not by applying induction to an excluded graph.

Primary source:
https://www.lirmm.fr/~pinlou/pdf/bklps17.pdf

## The central graph-theoretic inequality

For \(G-u\) not 2-connected, the block pieces \(G_i\) meet
only in \(u,v\); they have orders \(s_i\) satisfying
\[
\sum_{i=1}^{k}s_i=n+2(k-1).
\]
The lemma on good edges gives
\[
\eta(G)\ge\sum_{i=1}^{k}\eta(G_i)+k(k-1).
\]
The extra term comes from choosing one neighbor \(w_i\) of \(u\)
in every block and counting, for each unordered pair \((i,j)\),
**four** good edges for \(\{w_i,w_j\}\) compared with
graph distance **two**; its contribution is two.

For \(5\le s\le9\), the exact lower-bound table is
\[
(L(5),L(6),L(7),L(8),L(9))=(10,8,11,14,17).
\]
For \(s\ge10\), minimal-counterexample induction gives
\(L(s)=2s\) because every piece is 2-connected and
none of the three exceptional isomorphism types.
Define \(d(s)=L(s)-2s\); thus
\[
(d(5),d(6),d(7),d(8),d(9))=(0,-4,-3,-2,-1),
\qquad d(s)\ge0\quad(s\ge10).
\]
Substitution gives, with **no missing double counting**,
\[
\boxed{\eta(G)\ge
2n+(k-1)(k+4)+\sum_{i=1}^{k}d(s_i).}
\]

## Exhaustion of the purely arithmetic size cases

For \(k\ge3\), \(d(s_i)\ge-4\), hence
\[
\eta(G)-2n\ge(k-1)(k+4)-4k
=k^2-k-4\ge2.
\]

For \(k=2\), \((k-1)(k+4)=6\), and
\(s_1+s_2=n+2\). Sort \(s_1\le s_2\).

- If \(n\ge12\), then \(s_1+s_2\ge14\).
  If both are at most 9, the only pairs with sum
  at least 14 begin \((5,9),(6,8),(7,7)\);
  the corresponding deficits are \(-1,-6,-6\).
  Increasing either size among 5--9 cannot make
  the sum less than \(-6\) (check the five displayed
  deficits); if one is at least 10, the other has
  deficit at least \(-4\) and the larger one
  nonnegative. Thus
  \(d(s_1)+d(s_2)\ge-6\), yielding \(\eta(G)\ge2n\).
- If \(n=11\), then \(s_1+s_2=13\).
  The only pairs are \((5,8)\) and \((6,7)\).
  Their deficits are \(-2\) and \(-7\).
  Only \((6,7)\) needs a separate certificate,
  which the manuscript supplies (160 gluing
  configurations, minimum \(\eta=23\)).
- If \(n=10\), then \(s_1+s_2=12\).
  The only pairs are \((5,7)\) and \((6,6)\).
  Their deficits are \(-3\) and \(-8\).
  Only \((6,6)\) needs a separate certificate
  (105 gluing configurations, minimum \(\eta=20\)).

These are *all* possible block sizes because
every piece has order at least five.

## Complete induction gate

Order ten is checked separately by the finite base
certificate: 300,311 final candidate occurrences,
minimum \(\eta=20\), zero failures.
For \(n\ge11\), assume a minimum-order counterexample.
It has a distinct dominated pair by the published
no-domination lemma. Every possible deletion is
then one of:

1. complete: treated by the exact formula for \(K_n^t\);
2. 2-connected exceptional: treated by the two
   true-twin tables, *without* circular induction;
3. 2-connected nonexceptional: reduced by
   \(\eta(G)\ge\eta(G-u)+2\) to order \(n-1\);
4. not 2-connected: covered by the block inequality
   plus the 105/160 gluing certificates.

This is a genuinely exhaustive four-way case split
for the quoted external hypotheses. The exceptional
low-order proofs are not reused outside their stated
ranges.

## Audit boundary

The algebraic size exhaustion and formal induction
are justified by the argument above. The whole
theorem additionally relies on:
- the precise published lemmas and their appropriate
  **distinct-vertex** interpretation;
- the correctness and exhaustive input generation of
  the integer C++ certificate;
- the two true-twin polynomial classifications.

The original repository contains independent executable
certificates for the low-order graph statements.
Finite graph checks by themselves do not establish
the all-orders theorem; the noncomputational
four-way reduction above is essential.
