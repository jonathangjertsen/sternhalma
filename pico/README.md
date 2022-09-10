# Pico project base

Base for a project for Raspberry Pi Pico, using the C pico-sdk

Provides builds for Pico and Pico W

## Prerequisites

* [cmake](https://cmake.org/download/)
* [ninja](https://ninja-build.org/)
* [ARM GNU toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)

## Initial set-up

```sh
git clone --recurse-submodules <upstream-url>
sh scripts/setup.sh
```

## Build

```sh
sh scripts/build.sh
```
