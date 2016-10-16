#!/bin/bash

set -e
[ ! -r 'DEBUG.mak' ] && FEATURES='debug' ../configure.sh sources
make -f DEBUG.mak
[ ! -r 'RELEASE.mak' ] && FEATURES='release' ../configure.sh sources
make -f RELEASE.mak
