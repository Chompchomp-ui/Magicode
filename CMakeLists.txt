cmake_minimum_required(VERSION 3.5)

project(Magicode VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_STATIC_BUILD ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(SFML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/External/SFML-2.6.1/include")
set(SFML_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/External/SFML-2.6.1/lib")

set(TS_FILES Magicode_fr_FR.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Magicode
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        sfmlwidget.h sfmlwidget.cpp
        sfmlpreview.h sfmlpreview.cpp
        assets.qrc
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Magicode APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Magicode SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Magicode STATIC
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

#set(SFML_STATIC_LIBRARIES TRUE)
target_include_directories(Magicode PRIVATE ${SFML_INCLUDE_DIR})
link_directories(${SFML_LIBRARY_DIR})
target_compile_definitions(Magicode PRIVATE SFML_STATIC)

target_link_libraries(Magicode PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Magicode PRIVATE
    ${SFML_LIBRARY_DIR}/libsfml-graphics-s.a
    ${SFML_LIBRARY_DIR}/libsfml-window-s.a
    ${SFML_LIBRARY_DIR}/libsfml-system-s.a
)

if(WIN32)
    target_link_libraries(Magicode PRIVATE
        opengl32
        winmm
        gdi32
        ws2_32
    )
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Magicode)
endif()
set_target_properties(Magicode PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Magicode
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Magicode)
endif()
