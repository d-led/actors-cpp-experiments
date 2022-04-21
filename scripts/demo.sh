#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

export PATH=$PATH:$VCPKG_ROOT

ls -rtl $VCPKG_ROOT

# https://stackoverflow.com/a/246128/847349
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_DIR=build

$SCRIPT_DIR/get-deps.sh

$SCRIPT_DIR/build.sh

$SCRIPT_DIR/../$BUILD_DIR/actor-experiments-cpp
