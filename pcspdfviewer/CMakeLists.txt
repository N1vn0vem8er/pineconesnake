cmake_minimum_required(VERSION 3.19)
project(pcspdfviewer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Pdf PdfWidgets)

qt_standard_project_setup()

qt_add_executable(pcspdfviewer
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/pdfview.h src/pdfview.cpp
)

include_directories(
    src
)

target_link_libraries(pcspdfviewer
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Pdf
        Qt::PdfWidgets
)

include(GNUInstallDirs)

install(TARGETS pcspdfviewer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET pcspdfviewer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
