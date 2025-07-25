#include "MeshRender.h"
#include <d3dcompiler.h>



MeshRender::MeshRender(IDrawer* drawer, D3D11Context* context)
	: m_context(context)
	, m_drawer(drawer)
	, m_camera(nullptr) {

}

void MeshRender::render(double dt) {
	m_pipeline = std::make_shared<Pipeline>(m_context, m_material.get(), &m_vertex, m_drawer);

	if (m_update) {
		m_update(this, dt);
	}
	
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

void MeshRender::setRenderCamera(ICamera* camera) {
	m_camera = camera;
}
