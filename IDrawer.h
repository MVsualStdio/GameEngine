#pragma once

#include "D3D11Context.h"
#include <vector>
#include "Texture.h"
#include "Eigen/Core"
#include <memory>
#include <variant>
#include <functional>
class IRenderObject {
public:
	virtual void render() = 0;
};

class IDrawer {
public:
	using DrawFunction = std::function<void(IDrawer* draw)>;
	virtual void present(double dt) = 0;
	virtual void init(D3D11Context* context);
	void addItem(std::unique_ptr<IRenderObject> item);
	void renderForeach();
	virtual ComPtr<ID3D11RenderTargetView> getRenderTarget() = 0;
	void initDrawFunction(DrawFunction op);
	void initRender();
protected:
	std::vector<std::unique_ptr<IRenderObject>> m_items;
	D3D11Context* m_context = nullptr;
	DrawFunction m_op;
};

struct VertexUV {
	Eigen::Vector3<float> pos;
	Eigen::Vector3<float> normal;
	Eigen::Vector2<float> uv;
	Eigen::Vector4<float> color;
};

struct VertexUVData {
	std::vector<VertexUV> data;
	std::vector<uint32_t> index;
	size_t dataSize() {
		return data.size() * sizeof(VertexUV);
	}
	size_t itemSize() {
		return sizeof(VertexUV);
	}
	VertexUV* ptrData() {
		return data.data();
	}
	size_t indexSize() {
		return index.size() * sizeof(uint32_t);
	}
	uint32_t* ptrIndex() {
		return index.data();
	}
};

inline size_t StringToID(std::string_view str)
{
	static std::hash<std::string_view> hash;
	return hash(str);
}
//, Eigen::Matrix4f
using Property = std::variant<int, uint32_t, float, Eigen::Matrix4f>;