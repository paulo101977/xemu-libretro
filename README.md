# xemu-libretro

A fork of Xemu with the goal of creating a libretro core for Reinforcement Learning and RetroArch

## Build on Linux

```shell
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(getconf _NPROCESSORS_ONLN)
```

## Build on windows

```shell
cmake -S . -B build -T ClangCL -DCMAKE_BUILD_TYPE=Release
cmake --build build --target xemu-libretro --config Release
```

## Build for windows with MinGW64 with WSL2

```shell
sudo apt update
sudo apt install -y build-essential python3 python3-pip meson ninja-build pkg-config git libglib2.0-dev libepoxy-dev libsdl2-dev
...
#TODO add complete command for windows here
```

<!-- ## Build on windows with MinGW64

```shell
cd Xbox/
./configure --enable-debug --disable-werror
python scripts/qapi-gen.py --output-dir=build/qapi qapi/qapi-schema.json

cd ..

cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --target xemu-libretro --config Release
``` -->
