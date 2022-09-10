cmake_minimum_required(VERSION 3.12)

# Canonicalize path
set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor/pico-sdk)
get_filename_component(PICO_SDK_PATH "${PICO_SDK_PATH}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
if (NOT EXISTS ${PICO_SDK_PATH})
    message(FATAL_ERROR "Directory '${PICO_SDK_PATH}' not found")
endif ()

# Get SDK include-file
set(PICO_SDK_INIT_CMAKE_FILE ${PICO_SDK_PATH}/pico_sdk_init.cmake)
if (NOT EXISTS ${PICO_SDK_INIT_CMAKE_FILE})
    message(FATAL_ERROR "Directory '${PICO_SDK_PATH}' does not appear to contain the Raspberry Pi Pico SDK")
endif ()

# Pull in SDK (must be before project)
include(${PICO_SDK_INIT_CMAKE_FILE})

# Global compile options
add_compile_options(
  -Wl,-static
  -Wl,--gc-sections
  -fdata-sections
  -ffunction-sections
  -Wall
  -Wextra
  -Werror
  -fdiagnostics-color=always
  -Wno-unused-parameter
)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-volatile")

# Set up project
project(pico_project_base C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Initialize the SDK
pico_sdk_init()

add_subdirectory(application)
add_subdirectory(lib)
