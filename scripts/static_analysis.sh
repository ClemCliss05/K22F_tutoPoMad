#!/bin/bash
set -e

./scripts/clang-format.sh
./scripts/clang-tidy.sh
./scripts/cppcheck.sh