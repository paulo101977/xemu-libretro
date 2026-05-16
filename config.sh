#!/usr/bin/env bash
#cd Xbox

#rm -rf build

#export CFLAGS="-Wno-error=old-style-definition"
#export CXXFLAGS="-Wno-error=old-style-definition"
#export LDFLAGS="-lcurl $LDFLAGS"
#export LIBS="-lcurl $LIBS"
#export MESON=/mingw64/bin/meson
#export NINJA=/mingw64/bin/ninja
#export PKG_CONFIG=x86_64-w64-mingw32-pkg-config
#export PKG_CONFIG_PATH=/usr/x86_64-w64-mingw32/sys-root/mingw/lib/pkgconfig

#./build.sh -p win64-cross

#cd build
# ninja xemu-libretro.dll
#if [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
    # Windows/MinGW
    #ninja xemu_module.pyd
    #cp xemu_module.pyd ../../xemu_module.pyd
#elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    #ninja xemu_module.so
    #cp xemu_module.so ../../xemu_module.so
#else
    # Fallback
    #ninja xemu_module
#fi

#cd ../..

#!/usr/bin/env bash
cd Xbox

rm -rf build

platform="${OSTYPE}"

echo "Detected platform: $platform"

# defaults
unset PKG_CONFIG
unset PKG_CONFIG_PATH
unset MESON
unset NINJA

export CFLAGS="-Wno-error=old-style-definition"
export CXXFLAGS="-Wno-error=old-style-definition"

export LDFLAGS="-lcurl $LDFLAGS"
export LIBS="-lcurl $LIBS"

if [[ "$platform" == "linux-gnu"* ]]; then
    echo ">>> Linux native build"

    export PKG_CONFIG="pkg-config"

    ./build.sh

elif [[ "$platform" == "msys" || "$platform" == "cygwin" ]]; then
    echo ">>> Windows MSYS2 build"

    export PKG_CONFIG=x86_64-w64-mingw32-pkg-config
    export PKG_CONFIG_PATH=/mingw64/lib/pkgconfig
    export MESON=/mingw64/bin/meson
    export NINJA=/mingw64/bin/ninja

    ./build.sh -p win64-cross

else
    echo "Unsupported platform: $platform"
    exit 1
fi

cd build

# build outputs
if [[ "$platform" == "linux-gnu"* ]]; then
    ninja xemu_module.so
    cp xemu_module.so ../../xemu_module.so
else
    ninja xemu_module.pyd
    cp xemu_module.pyd ../../xemu_module.pyd
fi

cd ../..
