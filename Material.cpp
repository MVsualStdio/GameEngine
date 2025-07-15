#include "Material.h"
#include <d3dcompiler.h>
#include <iostream>
Material::Material(D3D11Context* context)
	: m_context(context) {

}
Material::~Material() {

}
void Material::addTexture(std::string name, Texture2D texture) {
	if (m_textures.find(name) != m_textures.end()) {
		m_textures[name] = std::make_unique<Texture2D>(texture);
	}
	else {
		m_textures.emplace(name, std::make_unique<Texture2D>(texture));
	}
}

Texture2D Material::getTexture(std::string name) {
	if (m_textures.find(name) != m_textures.end()) {
		return *m_textures[name];
	}
	else {
		return Texture2D(m_context, 0, 0);
	}
}

void Material::setVSShader(LPCWSTR filename, LPCSTR entry) {
	ID3DBlob* ppErrorMsgs = nullptr;

	auto hr = D3DCompileFromFile(L"E:/LearnSomething/RTTR/HLSL/baseVS.hlsli", 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, m_VSBuffer.GetAddressOf(), &ppErrorMsgs);

	if (FAILED(hr))
	{
		if (ppErrorMsgs != nullptr)
		{
			std::cout << reinterpret_cast<const char*>(ppErrorMsgs->GetBufferPointer()) << std::endl;;
		}
	}

	hr = m_context->m_Device->CreateVertexShader(m_VSBuffer->GetBufferPointer(), m_VSBuffer->GetBufferSize(), NULL, m_VS.GetAddressOf());
}

void Material::setPSShader(LPCWSTR filename, LPCSTR entry) {
	ID3DBlob* ppErrorMsgs = nullptr;
	auto hr = D3DCompileFromFile(filename, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, m_PSBuffer.GetAddressOf(), &ppErrorMsgs);
	if (FAILED(hr))
	{
		if (ppErrorMsgs != nullptr)
		{
			std::cout << reinterpret_cast<const char*>(ppErrorMsgs->GetBufferPointer()) << std::endl;;
		}
	}
	hr = m_context->m_Device->CreatePixelShader(m_PSBuffer->GetBufferPointer(), m_PSBuffer->GetBufferSize(), NULL, m_PS.GetAddressOf());
}

HRESULT Material::shaderReflection(ID3DBlob* blob) {
	HRESULT hr;

	ComPtr<ID3D11ShaderReflection> pShaderReflection;
	hr = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D11ShaderReflection),
		reinterpret_cast<void**>(pShaderReflection.GetAddressOf()));
	for (uint32_t i = 0;; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC sibDesc;
		hr = pShaderReflection->GetResourceBindingDesc(i, &sibDesc);
		// 读取完变量后会失败，但这并不是失败的调用
		if (FAILED(hr)) {
			break;
		}
		if (sibDesc.Type == D3D_SIT_CBUFFER)
		{
			ID3D11ShaderReflectionConstantBuffer* pSRCBuffer = pShaderReflection->GetConstantBufferByName(sibDesc.Name);
			// 获取cbuffer内的变量信息并建立映射
			D3D11_SHADER_BUFFER_DESC cbDesc{};
			hr = pSRCBuffer->GetDesc(&cbDesc);
			if (FAILED(hr)) {
				return hr;
			}
			for (uint32_t j = 0; j < cbDesc.Variables; ++j)
			{
				ID3D11ShaderReflectionVariable* pSRVar = pSRCBuffer->GetVariableByIndex(j);
				D3D11_SHADER_VARIABLE_DESC svDesc;
				hr = pSRVar->GetDesc(&svDesc);
				if (FAILED(hr)) {
					return hr;
				}
			}
		}
	}
}

ID3D11VertexShader* Material::getVSShader() {
	return m_VS.Get();
}
ID3D11PixelShader* Material::getPSShader() {
	return m_PS.Get();
}
ID3DBlob* Material::getVSBlob() {
	return m_VSBuffer.Get();
}