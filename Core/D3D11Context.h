#pragma once

#include <cstdint>

#include <d3d11_4.h>

#include <wrl.h>
#include <memory>



using Microsoft::WRL::ComPtr;

class Texture2D;
class Depth2D;

class D3D11Context
{
public:
    D3D11Context(HWND windowHandle, uint32_t width, uint32_t height);
    virtual ~D3D11Context();

    void Present();
    void SetClearColor(float r, float g, float b, float a);
    virtual void OnResize(uint32_t width, uint32_t height);

    Texture2D* getScreenRT();
    Depth2D* getScreenDepth();
    void setOMScreenRT();
    void ClearScreenRT();
    void ClearScreenDepth(float Depth = 1.0f, UINT8 Stencil = 0);
    void ClearRT(ID3D11RenderTargetView* pRenderTargetView);
    void ClearDepth(ID3D11DepthStencilView* pRenderTargetView, float Depth = 1.0f, UINT8 Stencil = 0.0);
    void resetRT();
    uint32_t width() { return m_width; }
    uint32_t height() { return m_height; }
    float AspectRatio()const;
private:
    void CreateDeviceAndSwapchain(HWND windowHandle, uint32_t width, uint32_t height);
    void CreateAdapter();
    void CreateRenderTarget();
    void CreateViewport(uint32_t width, uint32_t height);

    float m_ClearColor[4];

    ComPtr<IDXGISwapChain> m_SwapChain;
    ComPtr<IDXGIAdapter1> m_Adapter;
    std::shared_ptr<Texture2D> m_RenderTarget;
    std::shared_ptr<Depth2D> m_Depth;
    //ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

    D3D11_VIEWPORT m_Viewport;
    D3D11_RECT m_ScissorRect;
    uint32_t m_width;
    uint32_t m_height;
public:
    ComPtr<ID3D11Device> m_Device;
    ComPtr<ID3D11DeviceContext> m_DeviceContext;
};


