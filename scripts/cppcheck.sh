#!/bin/bash

set -e

echo "Running cppcheck..."
echo "Report saved to build/build_cppcheck.txt"

mkdir -p build

FILES=$(find firmware tests \
    -type f \
    \( -name "*.cpp" -o -name "*.c" \) \
    -not -path "*/cmsis/*" \
    -not -path "*/linker/*" \
    -not -path "*/startup/*"
)

cppcheck \
    --enable=warning,performance,portability,style \
    --inconclusive \
    --std=c++17 \
    --force \
    --error-exitcode=1 \
    --suppress=functionStatic \
    $FILES \
    2> build/build_cppcheck.txt

echo "cppcheck done without error."