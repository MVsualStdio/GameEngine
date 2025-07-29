#pragma once

#include "D3D11Context.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "Eigen/Core"
#include "IDrawer.h"

class ConstantBufferVariable;

class CBufferData {
public:
	friend class ConstantBufferVariable;

	CBufferData() = default;
	~CBufferData() = default;

	CBufferData(const std::string& name, uint32_t startSlot, uint32_t byteWidth, BYTE* initData = nullptr);
	HRESULT CreateBuffer(ID3D11Device* device);
	void UpdateBuffer(ID3D11DeviceContext* deviceContext);
	ComPtr<ID3D11Buffer> getBuffer();
	uint32_t getSlot() { return startSlot; }

private:
	bool isDirty = false;
	ComPtr<ID3D11Buffer> cBuffer;
	std::vector<uint8_t> cbufferData;
	std::string cbufferName;
	uint32_t startSlot = 0;
};

struct ShaderResource
{
	std::string name;
	D3D11_SRV_DIMENSION dim;
	ComPtr<ID3D11ShaderResourceView> pSRV;
};

struct SamplerState {
	std::string name;
	ComPtr<ID3D11SamplerState> pSS;
};

class ConstantBufferVariable {
private:
	void SetFloatMatrix(uint32_t rows, uint32_t cols, const float* noPadData);
	void SetMatrixInBytes(uint32_t rows, uint32_t cols, const BYTE* noPadData);
private:
	struct PropertyFunctor
	{
		PropertyFunctor(ConstantBufferVariable& _cbv) : cbv(_cbv) {}
		void operator()(const Eigen::Matrix4f& val) { cbv.SetFloatMatrix(4, 4, val.data()); }
		void operator()(int value) {  }
		void operator()(float value) {  }
		void operator()(uint32_t value) {  }
		ConstantBufferVariable& cbv;
	};

public:
    ConstantBufferVariable() = default;
    ~ConstantBufferVariable() = default;

	ConstantBufferVariable(std::string name_, uint32_t offset, uint32_t size, CBufferData* pData);

	void Set(const Property& prop);

private:
	std::string name;
	uint32_t startByteOffset = 0;
	uint32_t byteWidth = 0;
	CBufferData* pCBufferData = nullptr;
};


struct VertexShaderInfo
{
	std::string name;
	ComPtr<ID3D11VertexShader> pVS;

	std::unique_ptr<CBufferData> pParamData = nullptr;
	std::unordered_map<size_t, std::shared_ptr<ConstantBufferVariable>> params;
};

