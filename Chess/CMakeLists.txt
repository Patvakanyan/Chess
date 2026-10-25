cmake_minimum_required(VERSION 3.16)

project(Chess LANGUAGES CXX)

enable_testing()

set(CHESS_SOURCES
    src/Board.cpp
    src/ChessUtils.cpp
    src/Game.cpp
    src/Move.cpp
    src/MoveHistory.cpp
    src/Piece.cpp
    src/Position.cpp
    src/Square.cpp
    src/Terminal.cpp
    src/exceptions/ChessException.cpp
    src/exceptions/EmptySquareException.cpp
    src/exceptions/InvalidMove.cpp
    src/exceptions/InvalidPositionException.cpp
    src/pieces/Bishop.cpp
    src/pieces/King.cpp
    src/pieces/Knight.cpp
    src/pieces/Pawn.cpp
    src/pieces/Queen.cpp
    src/pieces/Rook.cpp
)

add_library(ChessCore STATIC
    ${CHESS_SOURCES}
)

target_include_directories(ChessCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(ChessCore PUBLIC cxx_std_17)

target_compile_options(ChessCore
    PRIVATE
        -Wall
        -Wextra
        -Werror
        -g3
)

add_executable(Chess
    src/Main.cpp
)

target_link_libraries(Chess PRIVATE ChessCore)

target_compile_options(Chess
    PRIVATE
        -Wall
        -Wextra
        -Werror
        -g3
)

add_executable(Chess_tests
    tests/ChessUtils_test.cpp
)

target_link_libraries(Chess_tests PRIVATE ChessCore)

target_compile_options(Chess_tests
    PRIVATE
        -Wall
        -Wextra
        -Werror
        -g3
)

target_compile_features(Chess_tests PRIVATE cxx_std_17)

add_test(NAME Chess_tests COMMAND Chess_tests)
