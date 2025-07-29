#pragma once

#include "DrawManger.h"
#include "MeshRender.h"
#include "RenderPass.h"

class Scene : public IRenderObject {
public:
	using RenderItem = std::unordered_map<std::string, std::shared_ptr<MeshRender>>;
	Scene(D3D11Context* context, IDrawer* drawer);
	~Scene();
	void render(double dt) override;
	void updateCamera(ICamera* camera) override;
	void addMeshRender(const std::string& name, std::shared_ptr<MeshRender> mesh);
	std::shared_ptr<MeshRender> getRender(const std::string& name);
	const RenderItem& getAllRenderItems();
	void setDrawer(IDrawer* drawer);
private:
	D3D11Context* m_context;
	RenderItem m_renderItems;
	IDrawer* m_drawer;
};


class DrawMangerTexture : public DrawMangerBase {
public:
	DrawMangerTexture();
private:
	void prepare() override;
	void initCompent() override;
private:
	void initDrawScreen(IDrawer* draw);
	void initDrawTexture(IDrawer* draw);
	std::shared_ptr<Texture2D> texture;
	double m_time = 0.0;
};