# description

## depedency

1. glfw
2. glm
3. OpenGL 4.1+

# install

1. compile and build with cmake

```shell
# create build folder with your os cmd
cd build
cmake ..
cmake --build .
cd ..
```

then cmake will download and config glm and glfw repo.

2. Run "ICVP_TEST" app in build folder

# Usage

1. press WASD keys to walk forward/backward/left/right
2. press left-ctrl key, then you can move mouse to change camera toward

# known compile issue

1. build glfw failed in ubuntu because of wayland-scanner:
   Try to use install `libwayland-dev` `libxkbcommon-dev` `xorg-dev`:
   ```shell
   sudo apt install libwayland-dev libxkbcommon-dev xorg-dev
   ```
   as [glfw-compile](https://www.glfw.org/docs/latest/compile.html) said (other linux os can also refer this).
