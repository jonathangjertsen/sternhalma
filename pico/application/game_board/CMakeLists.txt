add_library(game_board INTERFACE)
target_include_directories(game_board INTERFACE .)
target_link_libraries(game_board INTERFACE ws2812)
