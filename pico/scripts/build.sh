#!/usr/bin/env bash
set -eu -o pipefail

pushd build
ninja
popd

pushd build_w
ninja
popd
