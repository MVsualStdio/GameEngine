#pragma once

#include "D3D11Context.h"
#include <vector>
#include "Texture.h"
#include "Eigen/Core"
#include <memory>
#include <variant>
#include <functional>
#include "Vertex.h"
#include "Camera.h"

class MeshRender;

using Property = std::variant<int, uint32_t, float, Eigen::Matrix4f, Eigen::Vector3f, std::vector<Eigen::Matrix4f>>;

inline size_t StringToID(std::string_view str)
{
	static std::hash<std::string_view> hash;
	return hash(str);
}

class IRenderObject {
public:
	virtual void render(Camera* camera) = 0;
};

class IDrawer {
public:
	IDrawer(D3D11Context* context);
	using DrawFunction = std::function<void(IDrawer* draw)>;

	virtual void present() = 0;
	virtual void onDraw(Camera* camera);
	virtual void clear() = 0;

	void addItem(MeshRender* item);
	virtual Texture2D* getRenderTarget() = 0;
	virtual Depth2D* getDepthStencil() = 0;
	std::vector<MeshRender*> getAllItems();
protected:
	std::vector<MeshRender*> m_items;
	D3D11Context* m_context = nullptr;
};

class IAnim {
public:
	virtual void tick(double dt) = 0;
};


