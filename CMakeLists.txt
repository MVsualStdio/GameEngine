# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.10)

project(qt_d3d11_window LANGUAGES CXX)

include(E:/LearnSomething/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(d3d11window
    main.cpp
    D3D11Window.h D3D11Window.cpp
    D3D11Context.h D3D11Context.cpp
    DrawManger.h  DrawManger.cpp
    IDrawer.h IDrawer.cpp
    DrawScreen.h DrawScreen.cpp
)



# set_target_properties(d3d11window PROPERTIES
#     WIN32_EXECUTABLE TRUE
#     MACOSX_BUNDLE TRUE
# )

target_link_libraries(d3d11window 
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib winmm.lib)

