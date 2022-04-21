#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

# https://stackoverflow.com/a/246128/847349
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

cd $SCRIPT_DIR/..

BUILD_DIR=build

cmake -B $BUILD_DIR -S . -DCMAKE_TOOLCHAIN_FILE=$VCPKG_HOME/scripts/buildsystems/vcpkg.cmake

cmake --build $BUILD_DIR
