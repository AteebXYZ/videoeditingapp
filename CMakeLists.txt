cmake_minimum_required(VERSION 3.16)
project(qt_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Multimedia QuickControls2)

pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(SWSCALE REQUIRED libswscale)

qt_add_executable(qt_app
    main.cpp
    qml/resources.qrc
    backend.cpp

    # backend.h
    MediaPoolManager.h
    MediaPoolManager.cpp
)

target_include_directories(qt_app PRIVATE
    ${AVFORMAT_INCLUDE_DIRS}
    ${AVUTIL_INCLUDE_DIRS}
    ${AVCODEC_INCLUDE_DIRS}
    ${SWSCALE_INCLUDE_DIRS}
)

# Register the QML module (optional but recommended for clean structure)
# qt_add_qml_module(qt_app
# URI QtApp # 👈 can be any name (you’ll use it like `import QtApp`)
# VERSION 1.0
# QML_FILES
# qml/Main.qml
# qml/components/Menu.qml
# qml/components/MenuItem.qml
# )
target_link_libraries(qt_app PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Multimedia
    ${AVFORMAT_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${SWSCALE_LIBRARIES}
)
