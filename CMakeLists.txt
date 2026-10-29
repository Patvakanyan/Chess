cmake_minimum_required(VERSION 3.16)

project(chessGui_2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup(REQUIRES 6.10)

add_subdirectory(Chess)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/importedcontent)
endif()

qt_add_executable(appchessGui_2
    main.cpp
    src/ChessScene.cpp
    src/ChessView.cpp
    src/ChessView.hpp
    src/GameController.cpp
    src/GameController.hpp
    src/mainwindow.cpp
    src/mainwindow.h
)

set_target_properties(appchessGui_2 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appchessGui_2
    PRIVATE
        Qt6::Widgets
        ChessCore
)

install(TARGETS appchessGui_2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
