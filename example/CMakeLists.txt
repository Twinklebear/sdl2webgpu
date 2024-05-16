cmake_minimum_required(VERSION 3.27)
project(sdl2webgpu-example)

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(cmake/FetchDawn.cmake)
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

# Add the sdl2webgpu library, the extra argument is only needed because it's out
# of source of this example
add_subdirectory(.. sdl2webgpu)

add_executable(sdl2webgpu_example main.c)
target_link_libraries(sdl2webgpu_example PUBLIC sdl2webgpu)
set_target_properties(sdl2webgpu PROPERTIES C_STANDARD 17 C_STANDARD_REQUIRED
                                                          ON)

if(WIN32)
  target_compile_definitions(sdl2webgpu PUBLIC SDL_MAIN_HANDLED)
endif()
