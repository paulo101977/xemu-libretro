#!/usr/bin/env bash
cd Xbox

export PKG_CONFIG=x86_64-w64-mingw32-pkg-config
export PKG_CONFIG_PATH=/usr/x86_64-w64-mingw32/sys-root/mingw/lib/pkgconfig

./build.sh -p win64-cross

cd ..
