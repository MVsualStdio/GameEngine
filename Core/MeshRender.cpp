#include "MeshRender.h"
#include <d3dcompiler.h>



MeshRender::MeshRender(IDrawer* drawer, D3D11Context* context)
	: m_context(context)
	, m_drawer(drawer)
	, m_camera(nullptr)
	, m_needUpdateUniform(false){

	m_update = [this](MeshRender* /*render*/, double dt) {
		this->tick(dt); 
	};

	m_cameraUpdate = [this](MeshRender* /*render*/) {
		m_needUpdateUniform = true;
		this->cameraChange();
	};
}

void MeshRender::render(double dt) {
	m_pipeline = std::make_shared<Pipeline>(m_context, m_material.get(), &m_vertex, m_drawer);
	m_update(this, dt);

	m_pipeline->IA();
	m_pipeline->VS();
	m_pipeline->Rasterizer();
	m_pipeline->PS();
	m_pipeline->OM();
	m_pipeline->DrawIndex();
}

void MeshRender::updateCamera(ICamera* camera) {
	setRenderCamera(camera);
	m_cameraUpdate(this);
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

void MeshRender::setUpdateUniform(UpdateUniform update) {
	m_cameraUpdate = update;
}

void MeshRender::setRenderCamera(ICamera* camera) {
	m_camera = camera;
}

void MeshRender::tick(double dt) {

}

void MeshRender::cameraChange() {

}
