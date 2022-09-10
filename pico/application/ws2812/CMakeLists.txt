add_library(ws2812 ws2812.cpp)
target_link_libraries(ws2812
    PRIVATE
        hardware_pio
        pico_stdlib
)
target_include_directories(ws2812 PUBLIC .)
