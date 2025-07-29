#pragma once

#include "D3D11Context.h"
#include "Material.h"
#include "IDrawer.h"
#include "Pipeline.h"
#include "Camera.h"


class MeshRender : public IRenderObject, public IAnim {
public:
	using UpdateFunction = std::function<void (MeshRender*, double )>;
	using UpdateUniform = std::function<void(MeshRender*)>;

	MeshRender(IDrawer* drawer, D3D11Context* context);

	void setMaterial(std::shared_ptr<Material> material);
	void setVertex(VertexUVData vertex);
	void setUpdateFun(UpdateFunction update);
	void setUpdateUniform(UpdateUniform update);
	void setRenderCamera(ICamera* camera);
	Material* getMaterial() { return m_material.get(); }
	ICamera* getRenderCamera() { return m_camera; }

	void render(double dt) override;
	void updateCamera(ICamera* camera) override;

	void tick(double dt) override;
	void cameraChange() override;

protected:
	D3D11Context* m_context;
	IDrawer* m_drawer;
	ICamera* m_camera;

private:
	std::shared_ptr<Pipeline> m_pipeline;
	std::shared_ptr<Material> m_material;
	VertexUVData m_vertex;
	bool m_needUpdateUniform;
protected:
	UpdateFunction m_update;
	UpdateUniform m_cameraUpdate;
};