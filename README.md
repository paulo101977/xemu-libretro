# xemu-libretro

A fork of Xemu with the goal of creating a libretro core for Reinforcement Learning and RetroArch

## Build on Linux

```shell
mkdir build && cd build
cmake .. -DLIBRETRO=ON -DCMAKE_BUILD_TYPE=Release
make -j$(getconf _NPROCESSORS_ONLN)
```

## Build on windows

```shell
cmake -B build -DLIBRETRO=ON -DCMAKE_BUILD_TYPE=Release -G"Visual Studio 17 2022" -A x64
cmake --build build --target xemu-libretro --config Release
```

## Build on windows using ming

```shell
cmake -B build -DLIBRETRO=ON -DCMAKE_BUILD_TYPE=Release -G Ninja
ninja
```
