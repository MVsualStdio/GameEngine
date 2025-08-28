#pragma once 

#include "Vertex.h"

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

class VertexUVData : public VertexBuffer<VertexUV> {
public:
	const D3D11_INPUT_ELEMENT_DESC* layout() const override {
		return DefaultLayout;
	}

	const unsigned int layoutCount() const override {
		return ARRAYSIZE(DefaultLayout);
	}
};

const D3D11_INPUT_ELEMENT_DESC AnimationLayout[6] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_SINT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

struct VertexAnimation {
	Eigen::Vector3<float> pos;
	Eigen::Vector3<float> normal;
	Eigen::Vector2<float> uv;
	Eigen::Vector4<float> color;
	Eigen::Vector4<int> boneIndex = { -1.0f,-1.0f,-1.0f,-1.0f };
	Eigen::Vector4<float> widget;
};

class VertexAnimationData : public VertexBuffer<VertexAnimation> {
public:
	const D3D11_INPUT_ELEMENT_DESC* layout() const override {
		return AnimationLayout;
	}

	const unsigned int layoutCount() const override {
		return ARRAYSIZE(AnimationLayout);
	}
};