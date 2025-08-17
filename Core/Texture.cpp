#include "Texture.h"

Texture2DBase::Texture2DBase(D3D11Context* context) 
	: m_context(context) {

}

Texture2DBase::Texture2DBase(D3D11Context* context, const CD3D11_TEXTURE2D_DESC& texDesc, const CD3D11_SHADER_RESOURCE_VIEW_DESC& srvDesc)
	: m_context(context)
	, m_width(texDesc.Width)
	, m_height(texDesc.Height) {

	m_pTexture.Reset();
	m_pTextureSRV.Reset();
	context->m_Device->CreateTexture2D(&texDesc, nullptr, m_pTexture.GetAddressOf());
	D3D11_TEXTURE2D_DESC desc;
	m_pTexture->GetDesc(&desc);

	if ((desc.BindFlags & D3D11_BIND_SHADER_RESOURCE))
	{
		context->m_Device->CreateShaderResourceView(m_pTexture.Get(), &srvDesc, m_pTextureSRV.GetAddressOf());
	}
}

Texture2DBase::Texture2DBase(D3D11Context* context, ComPtr<ID3D11ShaderResourceView> pTextureSRV)
	: m_pTextureSRV(pTextureSRV)
	, m_context(context) {

	ID3D11Resource* pResource;
	m_pTextureSRV->GetResource(&pResource);  // 获取绑定的资源

	if (SUCCEEDED(pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&m_pTexture))) {
		D3D11_TEXTURE2D_DESC desc;
		m_pTexture->GetDesc(&desc);

		m_width = desc.Width;
		m_height = desc.Height;
	}
}

Texture2D::Texture2D(D3D11Context* context, ID3D11Texture2D* pTexture)
	: Texture2DBase(context) {
	context->m_Device->CreateRenderTargetView(pTexture, NULL, m_pTextureRTV.GetAddressOf());
}

Texture2D::Texture2D(D3D11Context* context, uint32_t width, uint32_t height, 
	DXGI_FORMAT format, uint32_t mipLevels, uint32_t bindFlags)
	: Texture2DBase(context, CD3D11_TEXTURE2D_DESC(format, width, height, 1, mipLevels, bindFlags),
		CD3D11_SHADER_RESOURCE_VIEW_DESC(D3D11_SRV_DIMENSION_TEXTURE2D, format)) {
	D3D11_TEXTURE2D_DESC desc;
	m_pTexture->GetDesc(&desc);

	m_mipLevels = desc.MipLevels;
	if (bindFlags & D3D11_BIND_RENDER_TARGET)
	{
		context->m_Device->CreateRenderTargetView(m_pTexture.Get(), nullptr, m_pTextureRTV.GetAddressOf());
	}

}

Texture2D::Texture2D(D3D11Context* context, ComPtr<ID3D11ShaderResourceView> pTextureSRV)
	:Texture2DBase(context, pTextureSRV) {

}

DXGI_FORMAT Depth2D::GetDepthTextureFormat(DepthStencilBitsFlag flag)
{
	switch (flag)
	{
	case DepthStencilBitsFlag::Depth_16Bits: return DXGI_FORMAT_R16_TYPELESS;
	case DepthStencilBitsFlag::Depth_24Bits_Stencil_8Bits: return DXGI_FORMAT_R24G8_TYPELESS;
	case DepthStencilBitsFlag::Depth_32Bits: return DXGI_FORMAT_R32_TYPELESS;
	case DepthStencilBitsFlag::Depth_32Bits_Stencil_8Bits_Unused_24Bits: return DXGI_FORMAT_R32G8X24_TYPELESS;
	default: return DXGI_FORMAT_UNKNOWN;
	}
}

DXGI_FORMAT Depth2D::GetDepthSRVFormat(DepthStencilBitsFlag flag)
{
	switch (flag)
	{
	case DepthStencilBitsFlag::Depth_16Bits: return DXGI_FORMAT_R16_FLOAT;
	case DepthStencilBitsFlag::Depth_24Bits_Stencil_8Bits: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case DepthStencilBitsFlag::Depth_32Bits: return DXGI_FORMAT_R32_FLOAT;
	case DepthStencilBitsFlag::Depth_32Bits_Stencil_8Bits_Unused_24Bits: return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
	default: return DXGI_FORMAT_UNKNOWN;
	}
}

DXGI_FORMAT Depth2D::GetDepthDSVFormat(DepthStencilBitsFlag flag)
{
	switch (flag)
	{
	case DepthStencilBitsFlag::Depth_16Bits: return DXGI_FORMAT_D16_UNORM;
	case DepthStencilBitsFlag::Depth_24Bits_Stencil_8Bits: return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case DepthStencilBitsFlag::Depth_32Bits: return DXGI_FORMAT_D32_FLOAT;
	case DepthStencilBitsFlag::Depth_32Bits_Stencil_8Bits_Unused_24Bits: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	default: return DXGI_FORMAT_UNKNOWN;
	}
}

Depth2D::Depth2D(D3D11Context* context, uint32_t width, uint32_t height,
	DepthStencilBitsFlag depthStencilBitsFlag, uint32_t bindFlags)
	: Texture2DBase(context,
		CD3D11_TEXTURE2D_DESC(GetDepthTextureFormat(depthStencilBitsFlag), width, height, 1, 1, bindFlags),
		CD3D11_SHADER_RESOURCE_VIEW_DESC(D3D11_SRV_DIMENSION_TEXTURE2D, GetDepthSRVFormat(depthStencilBitsFlag))) {
	if (bindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		CD3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc(D3D11_DSV_DIMENSION_TEXTURE2D, GetDepthDSVFormat(depthStencilBitsFlag));
		m_context->m_Device->CreateDepthStencilView(m_pTexture.Get(), &dsvDesc, m_pTextureDSV.GetAddressOf());
	}
}