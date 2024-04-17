cmake_minimum_required(VERSION 3.25.1)
project(victor_dmm)

set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/usr/local/Qt6")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        SerialPort
        REQUIRED)

set(MACOSX_BUNDLE_ICON_FILE AppIcon.icns)
set(App_Icon ${CMAKE_CURRENT_SOURCE_DIR}/images/AppIcon.icns
        CanvasWidget.cpp
        CanvasWidget.h
        MainWindow.cpp
        MainWindow.h)
set_source_files_properties(${App_Icon} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")

add_executable(victor_dmm MACOSX_BUNDLE ${App_Icon}
        resources.qrc
        main.cpp
)

target_link_libraries(victor_dmm
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::SerialPort
)
