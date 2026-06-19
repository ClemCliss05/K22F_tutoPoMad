#!/bin/bash
set -e

echo "Formatting all project code..."

FILES=$(find firmware tests \
    \( -name "*.cpp" -o -name "*.hpp" -o -name "*.c" -o -name "*.h" \) \
    -not -path "*/cmsis/*" \
    -not -path "*/linker/*" \
    -not -path "*/startup/*"
)

clang-format --verbose -i -style=file:scripts/config/clang-format.yaml $FILES

echo "Format done."