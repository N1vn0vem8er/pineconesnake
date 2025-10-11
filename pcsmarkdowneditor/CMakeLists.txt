cmake_minimum_required(VERSION 3.19)
project(pcsmarkdowneditor LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets WebEngineCore WebEngineWidgets Concurrent)

qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

include_directories(
    src
)

qt_add_executable(pcsmarkdowneditor
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/codeeditor.h src/codeeditor.cpp
    src/linenumberarea.h src/linenumberarea.cpp
    src/filesystemtree.h src/filesystemtree.cpp
    src/textinputdialog.h src/textinputdialog.cpp src/textinputdialog.ui
    resources/resources.qrc
    src/document.h src/document.cpp
    src/previewpage.h src/previewpage.cpp
    src/markdownsyntaxhighlighter.h src/markdownsyntaxhighlighter.cpp
)

target_link_libraries(pcsmarkdowneditor
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::WebEngineCore
        Qt::WebEngineWidgets
        Qt::Concurrent
)

include(GNUInstallDirs)

install(TARGETS pcsmarkdowneditor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET pcsmarkdowneditor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
