#pragma once

#include "D3D11Context.h"
#include "IDrawer.h"
#include "CBuffer.h"
#include <d3dcompiler.h>

class IShader {
public:
	using CBuffers = std::unordered_map<uint32_t, CBufferData>;
	using CBufferVariables = std::unordered_map<size_t, std::shared_ptr<ConstantBufferVariable>>;
public:
	IShader(D3D11Context* context, LPCWSTR filename, LPCSTR entry, LPCSTR entryTarget);
	void setUniform(std::string name, Property value);
	CBuffers getUniform();
	ID3DBlob* getBlob();
protected:
	HRESULT shaderReflection(ID3DBlob* blob, CBuffers& ConstantBuffers, CBufferVariables& ConstantBufferVariables);
protected:
	D3D11Context* m_context;
	CBuffers m_CBuffers;
	CBufferVariables m_ConstantBufferVariables;
	ComPtr<ID3DBlob> m_blob;

};

class VSShader : public IShader {
public:
	VSShader(D3D11Context* m_context, LPCWSTR filename, LPCSTR entry);
	ComPtr<ID3D11VertexShader> getVS() { return m_VS; }
private:
	ComPtr<ID3D11VertexShader> m_VS;
};

class PSShader : public IShader {
public:
	PSShader(D3D11Context* m_context, LPCWSTR filename, LPCSTR entry);
	ComPtr<ID3D11PixelShader> getPS() { return m_PS; }
private:
	ComPtr<ID3D11PixelShader> m_PS;
};