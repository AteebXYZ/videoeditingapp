cmake_minimum_required(VERSION 3.20)
project(qt_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 via vcpkg
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Multimedia QuickControls2 Widgets)

# Find FFmpeg via vcpkg
find_package(FFmpeg REQUIRED)

# Add your executable
qt_add_executable(qt_app
    main.cpp
    qml/resources.qrc
    backend.cpp
    MediaPoolManager.cpp
    MediaPoolManager.h
)

# Include FFmpeg headers
target_include_directories(qt_app PRIVATE
    ${FFmpeg_INCLUDE_DIRS}
)

# Link Qt and FFmpeg
target_link_libraries(qt_app PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Multimedia
    Qt6::Widgets
    FFmpeg::avcodec
    FFmpeg::avformat
    FFmpeg::avutil
    FFmpeg::swscale
)
