#!/bin/bash
set -e

echo "Running linting with clang-tidy..."

BUILD_DIR=build
TIDY_DIR=$BUILD_DIR/build_tidy

cmake -B $TIDY_DIR \
    -DANALYSIS=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

FILES=$(find firmware/core firmware/services \
    \( -name "*.cpp" -o -name "*.c" \))

for file in $FILES; do
    echo "---- $file ----"
    clang-tidy \
        -p $TIDY_DIR \
        --config-file=scripts/config/clang-tidy.yaml \
        "$file"
done

echo "Clang-tidy done."