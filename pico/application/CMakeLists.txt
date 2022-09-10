add_executable(application main.cpp)
pico_add_extra_outputs(application)
target_link_libraries(application PRIVATE onboard_led pico_stdlib ws2812)
add_subdirectory(ws2812)
