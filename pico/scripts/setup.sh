#!/usr/bin/env bash
set -eu -o pipefail

HERE=$(dirname $(readlink -f $0))
$SHELL $HERE/unsetup.sh

# Update submodules
# Could use --recursive, but tinyusb has tons of submodules that we don't want
git submodule update --init
pushd vendor/pico-sdk
git submodule update --init
popd

# Set up build directory
mkdir build
mkdir build_w

pushd build
cmake .. -G Ninja -DPICO_BOARD=pico
popd

pushd build_w
cmake .. -G Ninja -DPICO_BOARD=pico_w
popd
