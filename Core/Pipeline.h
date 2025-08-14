#pragma once

#include "D3D11Context.h"
#include "Material.h"
#include "IDrawer.h"

class Pipeline {
public:
	Pipeline(D3D11Context* context, Material* material, AnyVertexBuffer* vertex, IDrawer* drawer);
	virtual void IA();
	virtual void VS();
	virtual void Rasterizer();
	virtual void PS();
	virtual void OM();
	virtual void DrawIndex();
private:
	D3D11Context* m_context;
	Material* m_material;
	IDrawer* m_drawer;
	AnyVertexBuffer* m_vertex;

	ComPtr<ID3D11Buffer> pVertexBuffers = nullptr;
	ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
	std::vector<uint32_t> strides;
	std::vector<uint32_t> offsets;
	uint32_t indexCount = 0;
	ID3D11InputLayout* pInputLayout = nullptr;
	ComPtr<ID3D11SamplerState> pSampleState = nullptr;
};