#include "MeshRender.h"
#include <d3dcompiler.h>



MeshRender::MeshRender(IDrawer* drawer, D3D11Context* context)
	: m_context(context)
	, m_drawer(drawer)
	, m_needUpdateUniform(true)
	, m_camera(nullptr)
	, m_world(Eigen::Matrix4f::Identity()) {

	m_update = [this](MeshRender* /*render*/, double dt) {
		this->tick(dt); 
	};

	m_cameraUpdate = [this](MeshRender* /*render*/, ICamera* camera) {
		this->cameraChange(camera);
	};
}

void MeshRender::render(double dt) {
	m_pipeline = std::make_shared<Pipeline>(m_context, m_material.get(), &m_vertex, m_drawer);
	m_update(this, dt);

	m_cameraUpdate(this, m_camera);
	//if (m_needUpdateUniform) {
	//	m_cameraUpdate(this, m_camera);
	//	m_needUpdateUniform = false;
	//}

	m_pipeline->IA();
	m_pipeline->VS();
	m_pipeline->Rasterizer();
	m_pipeline->PS();
	m_pipeline->OM();
	m_pipeline->DrawIndex();
}

void MeshRender::setMaterial(std::shared_ptr<Material> material) {
	m_material = material;
}

void MeshRender::setVertex(VertexUVData vertex) {
	m_vertex = vertex;
}

void MeshRender::setUpdateFun(UpdateFunction update) {
	m_update = update;
}

void MeshRender::setUpdateCamera(UpdateUniform update) {
	m_cameraUpdate = update;
}

void MeshRender::setWorld(Eigen::Matrix4f& world) {
	m_world = world;
}

void MeshRender::setCamera(ICamera* camera) {
	m_camera = camera;
}

void MeshRender::tick(double dt) {

}

void MeshRender::cameraChange(ICamera* camera) {

}
