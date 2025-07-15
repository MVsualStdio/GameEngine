#pragma once

#include "D3D11Context.h"
#include "Material.h"
#include "IDrawer.h"
#include "Pipeline.h"




class MeshRender : public IRenderObject {
public:
	MeshRender(IDrawer* drawer, D3D11Context* context);
	void render() override;
	void setWorld(const Eigen::Matrix4f world);
	void setView(const Eigen::Matrix4f view);
	void setProjection(const Eigen::Matrix4f projection);
	void setMaterial(std::shared_ptr<Material> material);
private:
	D3D11Context* m_context;
	IDrawer* m_drawer;

	std::shared_ptr<Pipeline> m_pipeline;
	std::shared_ptr<Material> m_material;

	Eigen::Matrix4f m_world;
	Eigen::Matrix4f m_view;
	Eigen::Matrix4f m_projection;
};