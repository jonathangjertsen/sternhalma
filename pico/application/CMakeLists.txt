add_executable(application
    main.cpp
    hw_leds.cpp
)
pico_add_extra_outputs(application)

target_link_libraries(application PRIVATE onboard_led pico_stdlib ws2812 pico_mcp23017 game_board)

add_subdirectory(ws2812)
add_subdirectory(debounce)
add_subdirectory(game_board)
