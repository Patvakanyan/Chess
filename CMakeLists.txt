cmake_minimum_required(VERSION 3.16)

project(chessGui_2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

enable_testing()

find_package(Qt6 6.4 QUIET COMPONENTS Widgets Multimedia)
if (NOT Qt6_FOUND)
    message(FATAL_ERROR
        "Qt >= 6.4 with Widgets and Multimedia is required. "
        "Run ./build.sh to install missing dependencies and build on supported Linux distributions. "
        "For a custom Qt installation, set CMAKE_PREFIX_PATH to its installation prefix. "
        "${Qt6_NOT_FOUND_MESSAGE}"
    )
endif()

qt_standard_project_setup()

add_subdirectory(Chess)
add_subdirectory(server)
add_subdirectory(client)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/importedcontent)
endif()

qt_add_executable(appchessGui_2
    main.cpp
    src/ChessScene.cpp
    src/ChessScene.hpp
    src/ChessView.cpp
    src/PromotionDialog.hpp
    src/PromotionDialog.cpp
    src/ChessView.hpp
    src/GameController.cpp
    src/GameController.hpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/GameMode.hpp
    src/StartPage.cpp
    src/StartPage.hpp
    src/SettingsPage.cpp
    src/SettingsPage.hpp
)

qt_add_resources(appchessGui_2 move_sounds
    PREFIX "/"
    FILES sound/castle.wav
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
        Qt6::Multimedia
        ChessCore
)

install(TARGETS appchessGui_2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
