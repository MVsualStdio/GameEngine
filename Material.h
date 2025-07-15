#pragma once

#include "D3D11Context.h"
#include "Texture.h"
#include <unordered_map>
#include <memory>

class Material {
public:
	Material(D3D11Context* context);
	~Material();
	void addTexture(std::string name, Texture2D texture);
	Texture2D getTexture(std::string name);
	void setVSShader(LPCWSTR filename, LPCSTR entry = "VS");
	void setPSShader(LPCWSTR filename, LPCSTR entry = "PS");
	ID3D11VertexShader* getVSShader();
	ID3D11PixelShader* getPSShader();
	ID3DBlob* getVSBlob();
private:
	HRESULT shaderReflection(ID3DBlob* blob);
private:
	D3D11Context* m_context;
	std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_textures;
	//std::unordered_map<std::string, std::any> m_uniform;

	ComPtr<ID3D11VertexShader> m_VS;
	ComPtr<ID3D11PixelShader> m_PS;
	ComPtr<ID3DBlob> m_VSBuffer;
	ComPtr<ID3DBlob> m_PSBuffer;
};