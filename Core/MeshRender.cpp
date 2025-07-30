#include "MeshRender.h"
#include <d3dcompiler.h>



MeshRender::MeshRender(IDrawer* drawer, D3D11Context* context)
	: m_context(context)
	, m_drawer(drawer)
	, m_needUpdateUniform(true)
	, m_view(Eigen::Matrix4f::Identity())
	, m_world(Eigen::Matrix4f::Identity()) 
	, m_projection(Eigen::Matrix4f::Identity()) {

	m_update = [this](MeshRender* /*render*/, double dt) {
		this->tick(dt); 
	};

	m_cameraUpdate = [this](MeshRender* /*render*/,const Eigen::Matrix4f& view, const Eigen::Matrix4f& projection) {
		this->cameraChange(view, projection);
	};
}

void MeshRender::render(double dt) {
	m_pipeline = std::make_shared<Pipeline>(m_context, m_material.get(), &m_vertex, m_drawer);
	m_update(this, dt);

	if (m_needUpdateUniform) {
		m_cameraUpdate(this, m_view, m_projection);
		m_needUpdateUniform = false;
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

void MeshRender::setUpdateCamera(UpdateUniform update) {
	m_cameraUpdate = update;
}

void MeshRender::setWorld(Eigen::Matrix4f& world) {
	m_world = world;
}

void MeshRender::setView(Eigen::Matrix4f& view) {
	if (view != m_view) {
		m_needUpdateUniform = true;
		m_view = view;
	}
}

void MeshRender::setProjection(Eigen::Matrix4f& projection) {
	if (projection != m_projection) {
		m_needUpdateUniform = true;
		m_projection = projection;
	}
}

void MeshRender::tick(double dt) {

}

void MeshRender::cameraChange(const Eigen::Matrix4f& view, const Eigen::Matrix4f& projection) {

}
