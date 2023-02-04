cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_SYSTEM_NAME Generic)
set(PICO_BOARD pico)
set(PICO_TOOLS_USE_STATIC_LIBSTDCPP 1)
add_subdirectory(pico)

project(sternhalma NONE)
enable_language(C CXX ASM)
add_library(sternhalma sternhalma/sternhalma.cpp)
target_include_directories(sternhalma PUBLIC sternhalma)

