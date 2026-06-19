#!/bin/bash
set -e

echo "Running cppcheck..."
echo "Report save to build/build_cppcheck.txt"

BUILD_DIR=build

mkdir -p $BUILD_DIR
cppcheck \
    --enable=warning,performance,portability,style \
    --inconclusive \
    --std=c++17 \
    --force \
    --error-exitcode=1 \
    --quiet \
    --suppress=missingIncludeSystem \
    firmware/core firmware/services tests \
    2> build/build_cppcheck.txt

echo "cppcheck done with no issues found."