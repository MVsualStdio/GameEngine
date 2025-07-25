#pragma once
#include "D3D11Context.h"
#include "Eigen/Core"

const D3D11_INPUT_ELEMENT_DESC DefaultLayout[4] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

struct VertexUV {
	Eigen::Vector3<float> pos;
	Eigen::Vector3<float> normal;
	Eigen::Vector2<float> uv;
	Eigen::Vector4<float> color;
};


template<class Vertex>
class VertexData {
public:
	std::vector<Vertex> data;
	std::vector<uint32_t> index;
	size_t dataSize() {
		return data.size() * sizeof(Vertex);
	}
	size_t itemSize() {
		return sizeof(Vertex);
	}
	Vertex* ptrData() {
		return data.data();
	}
	size_t indexSize() {
		return index.size() * sizeof(uint32_t);
	}
	uint32_t* ptrIndex() {
		return index.data();
	}
	virtual const D3D11_INPUT_ELEMENT_DESC* layout() = 0;
	virtual const unsigned int layoutSize() = 0;
};

class VertexUVData : public VertexData<VertexUV> {
public:
	const D3D11_INPUT_ELEMENT_DESC* layout() override{
		return DefaultLayout;
	}
	const unsigned int layoutSize() override{
		return ARRAYSIZE(DefaultLayout);
	}
};