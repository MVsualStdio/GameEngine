#pragma once

# include "D3D11Context.h"

class Texture2DBase {
public:
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
	Texture2D(D3D11Context* device, uint32_t width, uint32_t height, 
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
		uint32_t mipLevels = 1,
		uint32_t bindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	~Texture2D() = default;
	ID3D11RenderTargetView* GetRenderTarget() { return m_pTextureRTV.Get(); }
protected:
	uint32_t m_mipLevels = 1;
	ComPtr<ID3D11RenderTargetView> m_pTextureRTV;
};


//class Shader {
//public:
//	Shader(LPCWSTR fileName);
//private:
//	ComPtr<ID3DBlob> m_PSBuffer;
//	LPCWSTR m_fileName;
//};
//
//
//class PSShader {
//
//};