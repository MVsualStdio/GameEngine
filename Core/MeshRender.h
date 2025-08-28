#pragma once

#include "D3D11Context.h"
#include "Material.h"
#include "IDrawer.h"
#include "Pipeline.h"
#include "Camera.h"
#include <unordered_set>

class MeshRender : public Component {
public:
	using CameraChangeFunction = std::function<void(MeshRender*, Camera* )>;
	MeshRender();
	void init(IDrawer* drawer, D3D11Context* context);

	IDrawer* getPass() { return m_drawer; }
	void setMaterial(std::shared_ptr<Material> material);
	Material* getMaterial() { return m_material.get(); }

	void setVertex(std::shared_ptr<AnyVertexBuffer> vertex);
	void setVertex(std::shared_ptr<AnyVertexBuffer> vertex, int index);

	void render(Camera* camera);
	void cameraRender(CameraChangeFunction op);
protected:
	D3D11Context* m_context;
	IDrawer* m_drawer;
	CameraChangeFunction m_cameraOp;
private:
	std::vector<std::shared_ptr<Pipeline>> m_pipelines;
	std::shared_ptr<Material> m_material;
	std::vector<std::shared_ptr<AnyVertexBuffer>> m_vertex;
	bool m_init = false;
protected:
	virtual void initPipeline(std::vector<std::shared_ptr<AnyVertexBuffer>> vertex);
};