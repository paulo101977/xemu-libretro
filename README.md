# xemu-libretro

A fork of Xemu with the goal of creating a libretro core for Reinforcement Learning and RetroArch

## Build for first time on windows with MinGW64 or on Linux

```shell
sudo apt update
sudo apt install -y build-essential python3 python3-pip meson ninja-build pkg-config git libglib2.0-dev libepoxy-dev libsdl2-dev
sudo apt install mingw-w64 mingw-w64-tools pkg-config-mingw-w64-x86-64
sudo apt install libglew-dev freeglut3-dev
sudo apt install -y  pkg-config libpcap-dev
sudo apt install -y libssl-dev
sudo apt install -y pybind11-dev python3-pybind11


chmod +x ./config.sh
./config.sh
```

## Note

The command `./config.sh` configure run the build process once

## Rebuild on windows with MinGW64 or on Linux

```shell
# on Linux:
ninja -C Xbox/build xemu_module.so

# on Windows:
ninja -C Xbox/build xemu_module.pyd
```

## Where is my lib?

Typically located in the Xbox/build folder with the name xemu_module.pyd or xemu_module.so on Linux. The process finishes by copying the file to the project root, but if this process fails, you will find it in the locations mentioned earlier.
