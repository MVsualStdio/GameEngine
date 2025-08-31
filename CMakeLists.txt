# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.10)

project(engine)
set(CMAKE_CXX_STANDARD 17)
include(D:/work/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Eigen3 REQUIRED)
find_package(rttr REQUIRED)
find_package(assimp REQUIRED)
 
set(CoreFiles     
    Core/D3D11Context.h Core/D3D11Context.cpp
    Core/DrawManger.h Core/DrawManger.cpp
    Core/IDrawer.h Core/IDrawer.cpp
    Core/DrawScreen.h Core/DrawScreen.cpp
    Core/MeshRender.h Core/MeshRender.cpp
    Core/Pipeline.h Core/Pipeline.cpp
    Core/Texture.h Core/Texture.cpp
    Core/Material.h Core/Material.cpp
    Core/CBuffer.h Core/CBuffer.cpp
    Core/Shader.h Core/Shader.cpp
    Core/MeshFilter.h Core/MeshFilter.cpp
    Core/Vertex.h
    Core/VertexLayout.h
    Core/Camera.h Core/Camera.cpp
    Core/DDSTextureLoader11.h Core/DDSTextureLoader11.cpp
    Core/RenderPass.h Core/RenderPass.cpp
    Core/InputControl.h Core/InputControl.cpp
    Core/Geometry.cpp
    Core/LoadScence.h  Core/LoadScence.cpp
    Core/Animation/KeyAnimation.h  Core/Animation/KeyAnimation.cpp
    Core/TextureManager.h Core/TextureManager.cpp
    Core/FileSystem.h Core/FileSystem.cpp
)

set(ComponentFiles
    Core/Component/Component.h Core/Component/Component.cpp
    Core/Component/GameObject.h Core/Component/GameObject.cpp
    Core/Component/Transform.h Core/Component/Transform.cpp
)

set(ShellFiles     
    Shell/DrawScreenManager.h Shell/DrawScreenManager.cpp
    Shell/XWoodCrateBox.h Shell/XWoodCrateBox.cpp
    Shell/XCameraObject.h Shell/XCameraObject.cpp
    Shell/XPlane.h Shell/XPlane.cpp
    Shell/XComponent/XLightCpt.h Shell/XComponent/XLightCpt.cpp
    Shell/XLight.h Shell/XLight.cpp
    Shell/XCarAnimation.h Shell/XCarAnimation.cpp
    Shell/XBoneAnimation.h Shell/XBoneAnimation.cpp
    Shell/ShadowMap.h Shell/ShadowMap.cpp
)

set(WindowFiles
    Window/D3D11Window.h Window/D3D11Window.cpp
)

set(ResourceFiles
    HLSL/baseVS.hlsli HLSL/basePS.hlsli
    HLSL/woodCrateBox.hlsli
    HLSL/woodCrateBoxLight.hlsli
    HLSL/floor.hlsli
    HLSL/light.hlsli
    HLSL/BoneAnimation.hlsli
)

source_group("Core Files" FILES ${CoreFiles})
source_group("Shell Files" FILES  ${ShellFiles})
source_group("Window Files" FILES ${WindowFiles})
source_group("HLSL Files" FILES ${ResourceFiles})
source_group("Component Files" FILES ${ComponentFiles})

add_executable(engineMain
    main.cpp
    ${CoreFiles}
    ${ShellFiles}
    ${WindowFiles}
    ${ResourceFiles}
    ${ComponentFiles}
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

    assimp::assimp

    d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib winmm.lib)
