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

using Property = std::variant<int, uint32_t, float, Eigen::Matrix4f, Eigen::Vector3f>;

inline size_t StringToID(std::string_view str)
{
	static std::hash<std::string_view> hash;
	return hash(str);
}

class IRenderObject {
public:
	virtual void render(double dt) = 0;

	virtual void setCamera(ICamera* camera) = 0;
};

class IDrawer {
public:
	IDrawer(D3D11Context* context);
	using DrawFunction = std::function<void(IDrawer* draw)>;

	virtual void present() = 0;
	virtual void onDraw(double dt) = 0;
	virtual void clear() = 0;

	void addItem(std::shared_ptr<IRenderObject> item);
	void renderForeach(double dt);
	virtual ComPtr<ID3D11RenderTargetView> getRenderTarget() = 0;
	void updateCamera(ICamera* camera);
	std::vector<std::shared_ptr<IRenderObject>> getAllItems();
protected:
	std::vector<std::shared_ptr<IRenderObject>> m_items;
	D3D11Context* m_context = nullptr;
};

class IAnim {
public:
	virtual void tick(double dt) = 0;
	virtual void cameraChange(ICamera* camera) = 0;
};


