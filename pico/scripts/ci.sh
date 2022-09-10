#!/usr/bin/env bash
set -eu -o pipefail

HERE=$(dirname $(readlink -f $0))

$SHELL $HERE/install_dependencies.sh
$SHELL $HERE/setup.sh
$SHELL $HERE/build.sh
