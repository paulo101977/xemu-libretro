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
# ninja xemu-libretro.dll
if [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
    # Windows/MinGW
    ninja xemu_module.pyd
    cp xemu_module.pyd ../../xemu_module.pyd
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    ninja xemu_module.so
    cp xemu_module.so ../../xemu_module.so
else
    # Fallback
    ninja xemu_module
fi

cd ../..
