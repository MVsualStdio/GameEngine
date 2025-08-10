#include <rttr/registration>

#include "MeshRender.h"
#include <d3dcompiler.h>
#include "Component/GameObject.h"


RTTR_REGISTRATION
{
	rttr::registration::class_<MeshRender>("MeshRender")
			.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

MeshRender::MeshRender()
	: m_context(nullptr) {
	m_cameraOp = [](MeshRender*, Camera*) {

	};
}

void MeshRender::init(IDrawer* drawer, D3D11Context* context) {
	m_context = context;
	m_drawer = drawer;
}

void MeshRender::cameraRender(MeshRender::CameraChangeFunction op) {
	m_cameraOp = op;
}

void MeshRender::render(Camera* camera) {
	m_pipeline = std::make_shared<Pipeline>(m_context, m_material.get(), &m_vertex, m_drawer);
	
	if (gameObject()->getLayer() & camera->getCullMask() == 0) {
		return;
	}

	if (m_drawer == camera->getRenderPass()) {
		m_cameraOp(this, camera);
		m_pipeline->IA();
		m_pipeline->VS();
		m_pipeline->Rasterizer();
		m_pipeline->PS();
		m_pipeline->OM();
		m_pipeline->DrawIndex();
	}
	
}

void MeshRender::setMaterial(std::shared_ptr<Material> material) {
	m_material = material;
}

void MeshRender::setVertex(VertexUVData vertex) {
	m_vertex = vertex;
}
