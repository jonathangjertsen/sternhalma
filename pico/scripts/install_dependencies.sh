#!/usr/bin/env bash
set -eu -o pipefail

# Get required packages
apt update
apt -qy install cmake ninja-build gcc-arm-none-eabi gcc-multilib
