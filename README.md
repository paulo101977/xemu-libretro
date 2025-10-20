# xemu-libretro

A fork of Xemu with the goal of creating a libretro core for Reinforcement Learning and RetroArch

## Build on Linux

```shell
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(getconf _NPROCESSORS_ONLN)
```

## Build for windows with MinGW64 with WSL2 (cross-build)

```shell
sudo apt update
sudo apt install -y build-essential python3 python3-pip meson ninja-build pkg-config git libglib2.0-dev libepoxy-dev libsdl2-dev
sudo apt install -y  pkg-config libpcap-dev

cd Xbox

python3 -m venv pyvenv
source pyvenv/bin/activate
pip install --upgrade pip
pip install pyyaml
chmod +x configure

./configure --target-list=x86_64-softmmu --enable-debug --disable-werror

...
#TODO add complete command for windows here
```
