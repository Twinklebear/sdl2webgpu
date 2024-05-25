# SDL2 WebGPU Extension

This is an extension to make it easy to write an [SDL2](https://www.libsdl.org/)
based application using WebGPU that runs in both native and
web (via Emscripten) environments.
This library is inspired by Elie Michel's
[glfw3webgpu](https://github.com/eliemichel/glfw3webgpu),
providing the same functionality but for SDL2 applications.

## Overview

The library provides one function that takes an WebGPU instance
and SDL2 Window and returns the WebGPU surface corresponding to
the window. **Note that in Emscripten environments your canvas
must have the id** `canvas`.

```c
WGPUSurface sdl2GetWGPUSurface(WGPUInstance instance, SDL_Window *window);
```

## Usage

The library is designed to be dropped into any CMake project, and doesn't
build as a standalone library (also see the example). Similar to
glfw3webgpu, you can:

- Fetch the library using FetchContent by including the provided
  [`FetchSDL2WebGPU.cmake`](FetchSDL2WebGPU.cmake) by adding the file to your
  project and adding `include(<path>/FetchSDL2WebGPU.cmake)` to your CMakeLists.
  This should be done after finding Dawn + SDL2.
- Add the library source files to your project via a submodule and include
  it in your build via `add_subdirectory(sdl2webgpu)`
- Copy the `sdl2webgpu.*` files into your project and copy the CMake
  code from [`CMakeLists.txt`](CMakeLists.txt) to build the files into
  your own CMakeLists.txt

## Example

With this library you can easily write a fully cross platform (native/web)
SDL2 + WebGPU application, with the library abstracting the platform specific
WebGPU context setup. The same code is available under [example/](example/),
which just builds the native versions. For a full example building an
SDL2 WebGPU application for web + native see my template repo:
https://github.com/Twinklebear/webgpu-cpp-wasm

```c
#include <SDL.h>
#include <webgpu/webgpu.h>
#include <stdio.h>

#include "sdl2webgpu.h"

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0){
    fprintf(stderr, "Failed to init SDL2\n");
    return -1;
  }

  SDL_Window *window = SDL_CreateWindow("SDL2 + WebGPU",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					1280,
					720,
					0);

  WGPUInstanceDescriptor desc = {};
  desc.nextInChain = NULL;
  WGPUInstance instance = wgpuCreateInstance(&desc);

  // Now use sdl2webgpu to make the surface
  WGPUSurface surface = sdl2GetWGPUSurface(instance, window);
  printf("Surface = %p\n", surface);

  // Run your app loop, either through emscripten set main loop
  // or via a loop til SDL is told to quit. See the example for
  // more details

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
```
