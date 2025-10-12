cmake_minimum_required(VERSION 3.16)
project(qt_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Widgets Quick Multimedia QuickControls2)

qt_add_executable(qt_app
    main.cpp
    qml/resources.qrc
    backend.cpp
    backend.h
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
    Qt6::Widgets
    Qt6::Multimedia
)
