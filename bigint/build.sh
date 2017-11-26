#!/bin/bash

set -e
arch=`uname -m`
if [ "$arch" = "i386" -o "$arch" = "i686" ]; then
 arch=x86
elif [ "$arch" = "x86_64" -o "$arch" = "amd64" ]; then
 arch=x86_64
elif [ "${arch:0:3}" = "arm" ]; then
 arch=arm
elif [ "$arch" = "aarch64" ]; then
 arch=arm64
else
 echo 'Warning: Unable to detect cpu arch' >&2
 arch=generic
fi

echo "Using arch $arch"
sources="sources.$arch"
[ ! -r 'DEBUG.mak' ] &&  FEATURES='debug generic' ../configure.sh $sources
make -f DEBUG.mak
[ ! -r 'RELEASE.mak' ] && FEATURES='release' ../configure.sh $sources
make -f RELEASE.mak
