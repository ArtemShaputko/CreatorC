cmake_minimum_required(VERSION 3.5)

project(CreatorC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core5Compat Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core5Compat Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CreatorC
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Resources.qrc

        scaner.h scaner.cpp
        linenumberarea.h
        codeeditor.h codeeditor.cpp

        Lexemes/token.h Lexemes/token.cpp

        blockcontent.h blockcontent.cpp

        Lexemes/typeanalyzer.h Lexemes/typeanalyzer.cpp

        Lexemes/structanalyzer.h Lexemes/structanalyzer.cpp
        Lexemes/enumanalyzer.h Lexemes/enumanalyzer.cpp
        Lexemes/unionanalyzer.h Lexemes/unionanalyzer.cpp

        Lexemes/intanalyzer.h Lexemes/intanalyzer.cpp

        documenthighlighter.h documenthighlighter.cpp
        Lexemes/charanalyzer.h Lexemes/charanalyzer.cpp

        Lexemes/floatanalyzer.h Lexemes/floatanalyzer.cpp
        Lexemes/doubleanalyzer.h Lexemes/doubleanalyzer.cpp
        Lexemes/customtypeanalyzer.h Lexemes/customtypeanalyzer.cpp
        Lexemes/voidanalyzer.h Lexemes/voidanalyzer.cpp
        error.h error.cpp
        action.h action.cpp
        stack.h
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CreatorC APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CreatorC SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CreatorC
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CreatorC PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CreatorC PRIVATE Qt6::Core5Compat)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CreatorC)
endif()
set_target_properties(CreatorC PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CreatorC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CreatorC)
endif()
