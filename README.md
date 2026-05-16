# xemu-libretro

A fork of Xemu with the goal of creating a libretro core for Reinforcement Learning and RetroArch

## Build for first time on windows with MinGW64 or on Linux

```shell
sudo apt update
sudo apt install -y build-essential python3 python3-pip meson ninja-build pkg-config git libglib2.0-dev libepoxy-dev libsdl2-dev
sudo apt install mingw-w64 mingw-w64-tools pkg-config-mingw-w64-x86-64
sudo apt install -y  pkg-config libpcap-dev
sudo apt install -y libssl-dev
sudo apt install -y pybind11-dev python3-pybind11

# on windows
chmod +x ./config.sh
chmod +x ./Xbox/scripts/xemu-version.sh
./config.sh

# on linux
chmod +x ./config.sh
chmod +x ./Xbox/scripts/xemu-version.sh
chmod +x ./Xbox/scripts/configure.linux
./config.sh
```
