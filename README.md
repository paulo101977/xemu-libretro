# xemu-libretro

A fork of Xemu with the goal of creating a libretro core for Reinforcement Learning and RetroArch

## Build on Linux

```shell
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(getconf _NPROCESSORS_ONLN)
```

## Build for windows with MinGW64

```shell
# sudo apt update
# sudo apt install -y build-essential python3 python3-pip meson ninja-build pkg-config git libglib2.0-dev libepoxy-dev libsdl2-dev
# sudo apt install mingw-w64 mingw-w64-tools pkg-config-mingw-w64-x86-64
# sudo apt install -y  pkg-config libpcap-dev

export CMAKE_SOURCE_DIR={path_to_your_xemu_git}

chmod +x ./config.sh
./config.sh
```
