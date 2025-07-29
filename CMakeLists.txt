# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.10)

project(engine)
set(CMAKE_CXX_STANDARD 17)
include(E:/LearnSomething/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Eigen3 REQUIRED)
find_package(rttr REQUIRED)

add_executable(engineMain
    main.cpp
    D3D11Window.h D3D11Window.cpp
    D3D11Context.h D3D11Context.cpp
    DrawManger.h DrawManger.cpp
    IDrawer.h IDrawer.cpp
    DrawScreen.h DrawScreen.cpp
    MeshRender.h MeshRender.cpp
    Pipeline.h Pipeline.cpp
    Texture.h Texture.cpp
    Material.h Material.cpp
    CBuffer.h CBuffer.cpp
    Shader.h Shader.cpp
    MeshFilter.h MeshFilter.cpp
    Scene.h Scene.cpp
    Vertex.h
    Camera.h Camera.cpp
    DrawScreenManager.h DrawScreenManager.cpp
    GameObjBox.h GameObjBox.cpp
    RenderPass.h RenderPass.cpp
    DDSTextureLoader11.h DDSTextureLoader11.cpp
    HLSL/baseVS.hlsli HLSL/basePS.hlsli
)



# set_target_properties(d3d11window PROPERTIES
#     WIN32_EXECUTABLE TRUE
#     MACOSX_BUNDLE TRUE
# )

target_link_libraries(engineMain 
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets

    RTTR::Core

    Eigen3::Eigen

    d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib winmm.lib)
