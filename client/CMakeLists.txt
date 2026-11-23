cmake_minimum_required(VERSION 3.16)

project(ChessClient LANGUAGES CXX)

add_executable(chess_client
    main.cpp
    ChessClient.cpp
)

target_compile_features(chess_client PRIVATE cxx_std_17)

target_include_directories(chess_client PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set_target_properties(chess_client PROPERTIES
    CXX_EXTENSIONS OFF
)

target_compile_options(chess_client PRIVATE
    -Wall
    -Wextra
    -Wpedantic
	-Werror
)
