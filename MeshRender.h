#pragma once

#include "D3D11Context.h"
#include "Material.h"
#include "IDrawer.h"
#include "Pipeline.h"
#include "Camera.h"


class MeshRender : public IRenderObject{
public:
	using UpdateFunction = std::function<void (MeshRender*, double )>;

	MeshRender(IDrawer* drawer, D3D11Context* context);
	void render(double dt) override;
	void setMaterial(std::shared_ptr<Material> material);
	void setVertex(VertexUVData vertex);
	void setUpdateFun(UpdateFunction update);
	void setRenderCamera(ICamera* camera);
	Material* getMaterial() { return m_material.get(); }
	ICamera* getRenderCamera() { return m_camera; }

private:
	D3D11Context* m_context;
	IDrawer* m_drawer;
	ICamera* m_camera;

	std::shared_ptr<Pipeline> m_pipeline;
	std::shared_ptr<Material> m_material;
	VertexUVData m_vertex;
protected:
	UpdateFunction m_update;
};