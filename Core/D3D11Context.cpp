#include "D3D11Context.h"
#include <cassert>
#include "Texture.h"

D3D11Context::D3D11Context(HWND windowHandle, uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;
    m_ClearColor[0] = 0.0f;
    m_ClearColor[1] = 0.0f;
    m_ClearColor[2] = 0.0f;
    m_ClearColor[3] = 0.0f;

    CreateDeviceAndSwapchain(windowHandle,width,height);
    CreateAdapter();
    CreateRenderTarget();
    CreateViewport(width, height);
}

D3D11Context::~D3D11Context()
{
}

void D3D11Context::Present()
{
    m_SwapChain->Present(0, 0);
}

float D3D11Context::AspectRatio()const
{
    return static_cast<float>(m_width) / m_height;
}

void D3D11Context::OnResize(uint32_t width, uint32_t height)
{
    m_RenderTarget->GetRenderTarget().Reset();
    m_Depth->GetDepthStencil().Reset();
    
    m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    CreateRenderTarget();
    CreateViewport(width, height);
}

void D3D11Context::ClearRT(ID3D11RenderTargetView* pRenderTargetView) {
    m_DeviceContext->ClearRenderTargetView(pRenderTargetView, m_ClearColor);
}

void D3D11Context::ClearDepth(ID3D11DepthStencilView* pRenderTargetView, float Depth, UINT8 Stencil) {
    m_DeviceContext->ClearDepthStencilView(pRenderTargetView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, Depth, Stencil);
   
}

void D3D11Context::resetRT() {
    ID3D11RenderTargetView* output = nullptr;
    m_DeviceContext->OMSetRenderTargets(0, &output, nullptr);
}


void D3D11Context::ClearScreenRT()
{
    ClearRT(m_RenderTarget->GetRenderTarget().Get());
}

void D3D11Context::ClearScreenDepth(float Depth, UINT8 Stencil) {
    ClearDepth(m_Depth->GetDepthStencil().Get(), Depth, Stencil);
}

void D3D11Context::SetClearColor(float r, float g, float b, float a)
{
    m_ClearColor[0] = r;
    m_ClearColor[1] = g;
    m_ClearColor[2] = b;
    m_ClearColor[3] = a;
}

void D3D11Context::CreateDeviceAndSwapchain(HWND windowHandle, uint32_t width, uint32_t height)
{
    D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_1;
    UINT flags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = windowHandle;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    HRESULT hr;
    hr = D3D11CreateDeviceAndSwapChain
    (
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        flags,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &sd,
        m_SwapChain.GetAddressOf(),
        m_Device.GetAddressOf(),
        &fl,
        m_DeviceContext.GetAddressOf()
    );

    assert(hr == S_OK);
}

void D3D11Context::CreateAdapter()
{
    IDXGIFactory4* dxgiFactory = nullptr;
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    
    hr = dxgiFactory->EnumAdapters1(0, m_Adapter.GetAddressOf());
    assert(hr == S_OK);

#if 0
    auto adapterDescription = DXGI_ADAPTER_DESC1();
    m_Adapter->GetDesc1(&adapterDescription);
#endif

    dxgiFactory->Release();
}

void D3D11Context::CreateRenderTarget()
{
    ComPtr<ID3D11Texture2D> pBackBuffer;
    ComPtr<ID3D11RenderTargetView> RenderTargetView;
    m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    m_RenderTarget = std::make_shared<Texture2D>(this, pBackBuffer.Get());

    m_Depth = std::make_shared<Depth2D>(this, m_width, m_height);
}

void D3D11Context::CreateViewport(uint32_t width, uint32_t height)
{
    m_ScissorRect.left = m_Viewport.TopLeftX = 0;
    m_ScissorRect.top = m_Viewport.TopLeftY = 0;
    m_ScissorRect.right = m_Viewport.Width = (float) width;
    m_ScissorRect.bottom = m_Viewport.Height = (float) height;
    m_Viewport.MinDepth = .0f;
    m_Viewport.MaxDepth = 1.0f;
    m_DeviceContext->RSSetViewports(1, &m_Viewport);
}

Texture2D* D3D11Context::getScreenRT() {
    return m_RenderTarget.get();
}

Depth2D* D3D11Context::getScreenDepth() {
    return m_Depth.get();
}

void D3D11Context::setOMScreenRT() {
    m_DeviceContext->OMSetRenderTargets(1, m_RenderTarget->GetRenderTarget().GetAddressOf() , m_Depth->GetDepthStencil().Get());
}