#pragma once

#include <cstdint>

#include <d3d11_4.h>

#include <wrl.h>

using Microsoft::WRL::ComPtr;

class D3D11Context
{
public:
    D3D11Context(HWND windowHandle, uint32_t width, uint32_t height);
    virtual ~D3D11Context();

    void Present();
    void SetClearColor(float r, float g, float b, float a);
    virtual void OnResize(uint32_t width, uint32_t height);

    ComPtr<ID3D11RenderTargetView> getScreenRT();
    void setOMScreenRT();
    void ClearScreenRT();
private:
    void CreateDeviceAndSwapchain(HWND windowHandle, uint32_t width, uint32_t height);
    void CreateAdapter();
    void CreateRenderTarget();
    void CreateViewport(uint32_t width, uint32_t height);

    float m_ClearColor[4];

    ComPtr<IDXGISwapChain> m_SwapChain;
    ComPtr<IDXGIAdapter1> m_Adapter;
    ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

    D3D11_VIEWPORT m_Viewport;
    D3D11_RECT m_ScissorRect;
public:
    ComPtr<ID3D11Device> m_Device;
    ComPtr<ID3D11DeviceContext> m_DeviceContext;
};


