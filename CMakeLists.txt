# This project is set up to be added as a subdirectory within some larger
# project, not built on its own as a library.
add_library(sdl2webgpu STATIC sdl2webgpu.c)
target_include_directories(sdl2webgpu PUBLIC ${CMAKE_CURRENT_LIST_DIR})

if(NOT EMSCRIPTEN)
  target_link_libraries(sdl2webgpu PUBLIC SDL2::SDL2 webgpu)
  if(APPLE)
    target_compile_options(sdl2webgpu PRIVATE -x objective-c)
    target_link_libraries(
      sdl2webgpu PRIVATE "-framework Cocoa" "-framework CoreVideo"
                         "-framework IOKit" "-framework QuartzCore")
  endif()
elseif(EMSCRIPTEN)
  target_compile_options(sdl2webgpu PUBLIC --use-port=sdl2)
  target_link_options(sdl2webgpu PUBLIC -sUSE_SDL=2)
endif()
