#include "Texture.h"

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

