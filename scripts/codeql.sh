#!/bin/bash
set -e

BUILD_DIR=build
CODEQL_DIR=build/build_codeql

echo "Compile project"

cmake -B $CODEQL_DIR \
    -DANALYSIS=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

echo "Creating CodeQL database"

codeql database create $CODEQL_DIR/codeql-db \
    --language=cpp \
    --command="cmake --build $CODEQL_DIR -j2"

echo "Running security analysis"

codeql database analyze $CODEQL_DIR/codeql-db \
    codeql/cpp-queries:codeql-suites/cpp-security-extended.qls \
    --threads=1 \
    --ram=1500 \
    --format=sarif-latest \
    --output=$CODEQL_DIR/codeql-results.sarif

echo "Analysis finished"
echo "You can analyse codeql-results.sarif with the SARIF Viewer extension in VSC"