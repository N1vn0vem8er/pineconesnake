cmake_minimum_required(VERSION 3.19)
project(pcsvideoplayer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets MultimediaWidgets)

qt_standard_project_setup()

qt_add_executable(pcsvideoplayer
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
)

target_link_libraries(pcsvideoplayer
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::MultimediaWidgets
)

include_directories(
    src
)

include(GNUInstallDirs)

install(TARGETS pcsvideoplayer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET pcsvideoplayer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
