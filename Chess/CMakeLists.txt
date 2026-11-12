cmake_minimum_required(VERSION 3.16)

project(Chess LANGUAGES CXX)

enable_testing()

include(ExternalProject)

find_program(STOCKFISH_EXECUTABLE NAMES stockfish)
if (STOCKFISH_EXECUTABLE)
    message(STATUS "Using system Stockfish: ${STOCKFISH_EXECUTABLE}")
    set(CHESS_STOCKFISH_PATH "${STOCKFISH_EXECUTABLE}")
else()
    message(STATUS "Stockfish was not found; downloading and building it")

    set(STOCKFISH_SOURCE_DIR "${CMAKE_BINARY_DIR}/stockfish-source")
    set(STOCKFISH_BUILT_EXECUTABLE "${STOCKFISH_SOURCE_DIR}/src/stockfish")

    ExternalProject_Add(stockfish_external
        URL https://github.com/official-stockfish/Stockfish/archive/refs/tags/sf_17.1.tar.gz
        SOURCE_DIR "${STOCKFISH_SOURCE_DIR}"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -C "<SOURCE_DIR>/src" -j2 build ARCH=x86-64
        INSTALL_COMMAND ""
        BUILD_BYPRODUCTS "${STOCKFISH_BUILT_EXECUTABLE}"
    )

    set(CHESS_STOCKFISH_PATH "${STOCKFISH_BUILT_EXECUTABLE}")
endif()

set(CHESS_SOURCES
    src/Board.cpp
    src/utils/ChessAttackUtils.cpp
    src/utils/ChessGameRules.cpp
    src/utils/ChessPieceFactory.cpp
    src/utils/ChessFenUtils.cpp
    src/utils/ChessMoveUtils.cpp
    src/Game.cpp
    src/Move.cpp
    src/Piece.cpp
    src/Position.cpp
    src/Square.cpp
    src/Terminal.cpp
    src/exceptions/ChessException.cpp
    src/exceptions/EmptySquareException.cpp
    src/exceptions/InvalidMove.cpp
    src/exceptions/InvalidPositionException.cpp
	src/StockfishEngine.cpp
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

target_compile_definitions(ChessCore
    PRIVATE
        CHESS_STOCKFISH_PATH="${CHESS_STOCKFISH_PATH}"
)

if (TARGET stockfish_external)
    add_dependencies(ChessCore stockfish_external)
endif()

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
