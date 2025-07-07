#ifndef D3D11_CONTEXT_HPP
#define D3D11_CONTEXT_HPP

#include <cstdint>

#include <d3d11_4.h>

#include <wrl.h>

using Microsoft::WRL::ComPtr;

class D3D11Context
{
public:
    D3D11Context(HWND windowHandle, uint32_t width, uint32_t height);
    virtual ~D3D11Context();

    void ClearTarget();
    void SetClearColor(float r, float g, float b, float a);
    void ReceiveCommands();
    void DispatchCommands();
    void Draw(uint32_t elements);
    void NewFrame();
    void EndFrame();
    void Present();

    virtual void OnResize(uint32_t width, uint32_t height);

    // virtual std::any GetImGUIData() override;
private:
    void CreateDeviceAndSwapchain(HWND windowHandle, uint32_t width, uint32_t height);
    void CreateAdapter();
    void CreateRenderTarget();
    void CreateViewport(uint32_t width, uint32_t height);

    float m_ClearColor[4];

    ComPtr<ID3D11Device> m_Device;
    ComPtr<ID3D11DeviceContext> m_DeviceContext;
    ComPtr<IDXGISwapChain> m_SwapChain;
    ComPtr<IDXGIAdapter1> m_Adapter;
    ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

    D3D11_VIEWPORT m_Viewport;
    D3D11_RECT m_ScissorRect;
};



#endif //D3D11_CONTEXT_HPP
