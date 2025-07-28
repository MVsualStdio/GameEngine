#pragma once

#include "D3D11Context.h"
#include <vector>
#include "Texture.h"
#include "Eigen/Core"
#include <memory>
#include <variant>
#include <functional>
#include "Vertex.h"

using Property = std::variant<int, uint32_t, float, Eigen::Matrix4f>;

inline size_t StringToID(std::string_view str)
{
	static std::hash<std::string_view> hash;
	return hash(str);
}

class IRenderObject {
public:
	virtual void render(double dt) = 0;
	virtual void updateCamera() = 0;
};

class IDrawer {
public:
	using DrawFunction = std::function<void(IDrawer* draw)>;
	virtual void present(double dt) = 0;
	virtual void init(D3D11Context* context);
	void addItem(std::shared_ptr<IRenderObject> item);
	void renderForeach(double dt);
	virtual ComPtr<ID3D11RenderTargetView> getRenderTarget() = 0;
	void initDrawFunction(DrawFunction op);
	void initRender();
	void updateCamera();
protected:
	std::vector<std::shared_ptr<IRenderObject>> m_items;
	D3D11Context* m_context = nullptr;
	DrawFunction m_op;
};

class IAnim {
public:
	virtual void tick(double dt) = 0;
	virtual void cameraChange() = 0;
};


