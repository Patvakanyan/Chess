cmake_minimum_required(VERSION 3.16)

project(ChessServer LANGUAGES CXX)

find_package(Threads REQUIRED)

add_executable(chess_server
	main.cpp
	ChessServer.cpp
	ClientConnection.cpp
	GameSession.cpp
)

target_compile_features(chess_server PRIVATE cxx_std_17)

target_include_directories(chess_server PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(chess_server PRIVATE
	Threads::Threads
)

set_target_properties(chess_server PROPERTIES
	CXX_EXTENSIONS OFF
)

if(MSVC)
	target_compile_options(chess_server PRIVATE /W4)
else()
	target_compile_options(chess_server PRIVATE
		-Wall
		-Wextra
		-Wpedantic
		-Wpedantic
	)
endif()
