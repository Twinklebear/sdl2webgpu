#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <webgpu/webgpu.h>

#include "sdl2webgpu.h"

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    fprintf(stderr, "Failed to init SDL2\n");
    return -1;
  }

  SDL_Window *window =
      SDL_CreateWindow("SDL2 + WebGPU", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);

  WGPUInstanceDescriptor desc = {};
  desc.nextInChain = NULL;
  WGPUInstance instance = wgpuCreateInstance(&desc);

  // Now use sdl2webgpu to make the surface
  WGPUSurface surface = sdl2GetWGPUSurface(instance, window);
  printf("Surface = %p\n", surface);

  // Run your app loop, either through emscripten set main loop
  // or via a loop til SDL is told to quit. See the example for
  // more details
  // Note that in a web + native app we'd need to split out
  // the app loop to a separate function that we can pass
  // to emscripten set main loop
  bool quit = false;
  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
