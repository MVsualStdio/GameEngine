#include "MeshRender.h"
#include <d3dcompiler.h>



MeshRender::MeshRender(IDrawer* drawer, D3D11Context* context)
	: m_context(context)
	, m_drawer(drawer) {

}

void MeshRender::render() {
	m_pipeline = std::make_shared<Pipeline>(m_context, m_material.get(), m_drawer);
	//m_material
	m_pipeline->IA();
	m_pipeline->VS();
	m_pipeline->Rasterizer();
	m_pipeline->PS();
	m_pipeline->OM();
	m_pipeline->DrawIndex();
}

void MeshRender::setWorld(const Eigen::Matrix4f world) {
	m_world = world;
}

void MeshRender::setView(const Eigen::Matrix4f view) {
	m_view = view;
}

void MeshRender::setProjection(const Eigen::Matrix4f projection) {
	m_projection = projection;
}

void MeshRender::setMaterial(std::shared_ptr<Material> material) {
	m_material = material;
}