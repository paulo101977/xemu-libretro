#!/usr/bin/env bash
cd Xbox

rm -rf build

export CFLAGS="-Wno-error=old-style-definition"
export CXXFLAGS="-Wno-error=old-style-definition"
export LDFLAGS="-lcurl $LDFLAGS"
export LIBS="-lcurl $LIBS"
export MESON=/mingw64/bin/meson
export NINJA=/mingw64/bin/ninja
export PKG_CONFIG=x86_64-w64-mingw32-pkg-config
export PKG_CONFIG_PATH=/usr/x86_64-w64-mingw32/sys-root/mingw/lib/pkgconfig

./build.sh -p win64-cross

cd build
ninja xemu-libretro.dll

cd ../..
