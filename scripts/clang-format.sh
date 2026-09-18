#!/bin/bash
set -e

FILES=$(find firmware tests \
    \( -name "*.cpp" -o -name "*.hpp" -o -name "*.c" -o -name "*.h" \) \
    -not -path "*/cmsis/*" \
    -not -path "*/linker/*" \
    -not -path "*/startup/*"
)

if [[ "$1" == "--check" ]]; then

    echo "Checking formatting..."

    clang-format \
        --dry-run \
        --Werror \
        -style=file:scripts/config/clang-format.yaml \
        $FILES

else

    echo "Formatting project..."

    clang-format \
        --verbose \
        -i \
        -style=file:scripts/config/clang-format.yaml \
        $FILES

fi