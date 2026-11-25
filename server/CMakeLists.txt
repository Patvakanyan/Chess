cmake_minimum_required(VERSION 3.16)

project(ChessServer LANGUAGES CXX)

find_package(Threads REQUIRED)
include(CTest)

add_executable(chess_server
	main.cpp
	ChessServer.cpp
	ClientConnection.cpp
	GameSession.cpp
	../network/Protocol.cpp
)

target_compile_features(chess_server PRIVATE cxx_std_17)

target_include_directories(chess_server PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
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

if(BUILD_TESTING)
	add_executable(client_connection_tests
		tests/ClientConnection_test.cpp
		ClientConnection.cpp
		../network/Protocol.cpp
	)
	target_compile_features(client_connection_tests PRIVATE cxx_std_17)
	target_include_directories(client_connection_tests PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/.."
	)
	add_test(NAME client_connection_tests COMMAND client_connection_tests)
	set_tests_properties(client_connection_tests PROPERTIES TIMEOUT 15)
endif()
