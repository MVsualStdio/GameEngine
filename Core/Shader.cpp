#include "Shader.h"


IShader::IShader(D3D11Context* context, LPCWSTR filename, LPCSTR entry, LPCSTR entryTarget)
	: m_context(context) {
	ID3DBlob* ppErrorMsgs = nullptr;

	auto hr = D3DCompileFromFile(filename, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry, entryTarget, D3D10_SHADER_ENABLE_STRICTNESS, 0, m_blob.GetAddressOf(), &ppErrorMsgs);

	if (FAILED(hr))
	{
		if (ppErrorMsgs != nullptr)
		{
			//std::cout << reinterpret_cast<const char*>(ppErrorMsgs->GetBufferPointer()) << std::endl;;
		}
	}

}


void IShader::setUniform(std::string name, Property value) {
	size_t svNameID = StringToID(name);
	m_ConstantBufferVariables[svNameID]->Set(value);
}

void IShader::setTexture(size_t slot, Texture2D value) {
	m_TextureBuffer[slot].pSRV = value.GetShaderResource();
}

IShader::CBuffers IShader::getUniform() {
	return m_CBuffers;
}

IShader::TextureBuffer IShader::getTexture() {
	return m_TextureBuffer;
}

IShader::SampleBuffer IShader::getSample() {
	return m_SampleBuffer;
}

ID3DBlob* IShader::getBlob() {
	return m_blob.Get();
}


HRESULT IShader::shaderReflection(ID3DBlob* blob, CBuffers& ConstantBuffers, CBufferVariables& ConstantBufferVariables, TextureBuffer& textureBuffer, SampleBuffer& sampleBuffer) {
	HRESULT hr;

	ComPtr<ID3D11ShaderReflection> pShaderReflection;
	hr = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D11ShaderReflection),
		reinterpret_cast<void**>(pShaderReflection.GetAddressOf()));

	D3D11_SHADER_DESC sd;
	hr = pShaderReflection->GetDesc(&sd);
	if (FAILED(hr))
		return hr;
	for (uint32_t i = 0;; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC sibDesc;
		hr = pShaderReflection->GetResourceBindingDesc(i, &sibDesc);
		// 读取完变量后会失败，但这并不是失败的调用
		if (FAILED(hr)) {
			break;
		}
		if (sibDesc.Type == D3D_SIT_CBUFFER) {
			ID3D11ShaderReflectionConstantBuffer* pSRCBuffer = pShaderReflection->GetConstantBufferByName(sibDesc.Name);
			// 获取cbuffer内的变量信息并建立映射
			D3D11_SHADER_BUFFER_DESC cbDesc{};
			hr = pSRCBuffer->GetDesc(&cbDesc);
			if (FAILED(hr)) {
				return hr;
			}
			auto it = ConstantBuffers.find(sibDesc.BindPoint);
			if (it == ConstantBuffers.end())
			{
				ConstantBuffers.emplace(std::make_pair(sibDesc.BindPoint, CBufferData(sibDesc.Name, sibDesc.BindPoint, cbDesc.Size, nullptr)));
				ConstantBuffers[sibDesc.BindPoint].CreateBuffer(m_context->m_Device.Get());
			}

			for (uint32_t j = 0; j < cbDesc.Variables; ++j)
			{
				ID3D11ShaderReflectionVariable* pSRVar = pSRCBuffer->GetVariableByIndex(j);
				D3D11_SHADER_VARIABLE_DESC svDesc;
				hr = pSRVar->GetDesc(&svDesc);
				if (FAILED(hr)) {
					return hr;
				}
				size_t svNameID = StringToID(svDesc.Name);
				ConstantBufferVariables[svNameID] = std::make_shared<ConstantBufferVariable>(
					svDesc.Name, svDesc.StartOffset, svDesc.Size, &ConstantBuffers[sibDesc.BindPoint]);
			}
		}

		else if (sibDesc.Type == D3D_SIT_TEXTURE || sibDesc.Type == D3D_SIT_STRUCTURED || sibDesc.Type == D3D_SIT_BYTEADDRESS ||
			sibDesc.Type == D3D_SIT_TBUFFER) {
			auto it = textureBuffer.find(sibDesc.BindPoint);
			if (it == textureBuffer.end())
			{
				textureBuffer.emplace(std::make_pair(sibDesc.BindPoint,
					ShaderResource{ sibDesc.Name, sibDesc.Dimension, nullptr }));
			}
		}

		else if (sibDesc.Type == D3D_SIT_SAMPLER)
		{
			auto it = sampleBuffer.find(sibDesc.BindPoint);
			if (it == sampleBuffer.end())
			{
				sampleBuffer.emplace(std::make_pair(sibDesc.BindPoint,
					SamplerState{ sibDesc.Name, nullptr }));
			}
		}
	}
}

VSShader::VSShader(D3D11Context* m_context, LPCWSTR filename, LPCSTR entry)
	: IShader(m_context, filename, entry, "vs_5_0") {
	HRESULT hr = m_context->m_Device->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), NULL, m_VS.GetAddressOf());
	shaderReflection(m_blob.Get(), m_CBuffers, m_ConstantBufferVariables, m_TextureBuffer, m_SampleBuffer);
}

PSShader::PSShader(D3D11Context* m_context, LPCWSTR filename, LPCSTR entry)
	: IShader(m_context, filename, entry, "ps_5_0") {
	HRESULT hr = m_context->m_Device->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), NULL, m_PS.GetAddressOf());
	shaderReflection(m_blob.Get(), m_CBuffers, m_ConstantBufferVariables, m_TextureBuffer, m_SampleBuffer);
}