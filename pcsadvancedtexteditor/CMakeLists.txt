cmake_minimum_required(VERSION 3.16)

project(pcsadvancedtexteditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        linenumberarea.cpp
        linenumberarea.h
        savewarningdialog.cpp
        savewarningdialog.h
        savewarningdialog.ui
        searchwidget.cpp
        searchwidget.h
        searchwidget.ui
        tabwidget.cpp
        tabwidget.h
        texteditor.cpp
        texteditor.h
        texthighlighter.cpp
        texthighlighter.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pcsadvancedtexteditor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        globals.h globals.cpp
        settings.h settings.cpp
        spellcheckerworker.h spellcheckerworker.cpp
        filesystemtree.h filesystemtree.cpp
        textinputdialog.h textinputdialog.cpp textinputdialog.ui
        startwidget.h startwidget.cpp startwidget.ui
        mimefinderworker.h mimefinderworker.cpp
        structs.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET pcsadvancedtexteditor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(pcsadvancedtexteditor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(pcsadvancedtexteditor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(pcsadvancedtexteditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent ${HUNSPELL_INCLUDE_DIRS} hunspell)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.pcsadvancedtexteditor)
endif()
set_target_properties(pcsadvancedtexteditor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS pcsadvancedtexteditor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pcsadvancedtexteditor)
endif()
