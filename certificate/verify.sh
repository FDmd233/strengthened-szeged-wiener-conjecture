#!/bin/sh
set -eu
cd "$(dirname "$0")"

CXX=${CXX:-g++}
FLAGS="-O2 -std=c++17 -Wall -Wextra -Werror -pedantic"
rm -rf build
mkdir build

$CXX $FLAGS -Isrc src/self_test.cpp -o build/self_test
$CXX $FLAGS -Isrc src/base10_audit.cpp -o build/base10_audit
$CXX $FLAGS -Isrc src/piece_audit.cpp -o build/piece_audit
$CXX $FLAGS -Isrc src/glue_audit.cpp -o build/glue_audit

sha256sum -c SHA256SUMS.expected

{
  echo "[self-test]"
  ./build/self_test
  echo "[base10]"
  ./build/base10_audit
  echo "[pieces]"
  ./build/piece_audit
  echo "[glue]"
  ./build/glue_audit
} | tee RESULTS.txt

diff -u EXPECTED_RESULTS.txt RESULTS.txt
printf '%s\n' "certificate verification: PASS"
