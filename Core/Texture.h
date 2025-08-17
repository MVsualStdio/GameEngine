#pragma once

# include "D3D11Context.h"

class Texture2DBase {
public:
	Texture2DBase(D3D11Context* context);
	Texture2DBase(D3D11Context* context, ComPtr<ID3D11ShaderResourceView> pTextureSRV);
	Texture2DBase(D3D11Context* context, const CD3D11_TEXTURE2D_DESC& texDesc, const CD3D11_SHADER_RESOURCE_VIEW_DESC& srvDesc);
	virtual ~Texture2DBase() = default;
	ID3D11Texture2D* GetTexture() { return m_pTexture.Get(); }
	ID3D11ShaderResourceView* GetShaderResource() { return m_pTextureSRV.Get(); }
	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }
protected:
	ComPtr<ID3D11Texture2D> m_pTexture;
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV;
	D3D11Context* m_context;
	uint32_t m_width = 0;
	uint32_t m_height = 0;
};

class Texture2D : public Texture2DBase {
public:
	Texture2D(D3D11Context* context, ID3D11Texture2D* pTexture);
	Texture2D(D3D11Context* context, ComPtr<ID3D11ShaderResourceView> pTextureSRV);
	Texture2D(D3D11Context* device, uint32_t width, uint32_t height, 
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
		uint32_t mipLevels = 1,
		uint32_t bindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	~Texture2D() = default;
	ComPtr<ID3D11RenderTargetView> GetRenderTarget() { return m_pTextureRTV; }
protected:
	uint32_t m_mipLevels = 1;
	ComPtr<ID3D11RenderTargetView> m_pTextureRTV;
};

class Depth2D : public Texture2DBase
{
public:
	enum class DepthStencilBitsFlag
	{
		Depth_16Bits = 0,
		Depth_24Bits_Stencil_8Bits = 1,
		Depth_32Bits = 2,
		Depth_32Bits_Stencil_8Bits_Unused_24Bits = 3,
	};
	
	Depth2D(D3D11Context* context, uint32_t width, uint32_t height,
		DepthStencilBitsFlag depthStencilBitsFlag = DepthStencilBitsFlag::Depth_24Bits_Stencil_8Bits,
		uint32_t bindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);
	~Depth2D() override = default;

	static DXGI_FORMAT GetDepthTextureFormat(DepthStencilBitsFlag flag);
	static DXGI_FORMAT GetDepthSRVFormat(DepthStencilBitsFlag flag);
	static DXGI_FORMAT GetDepthDSVFormat(DepthStencilBitsFlag flag);

	
	ComPtr<ID3D11DepthStencilView> GetDepthStencil() { 
		return m_pTextureDSV;
	}
private:
	ComPtr<ID3D11DepthStencilView> m_pTextureDSV;
};