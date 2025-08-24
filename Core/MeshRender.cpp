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

void MeshRender::initPipeline(std::vector<std::shared_ptr<AnyVertexBuffer>> vertex) {
	size_t vertexSize = vertex.size();
	m_pipelines.resize(vertexSize);
	for (int i = 0; i < vertexSize;++i) {
		m_pipelines[i] = std::make_shared<Pipeline>(m_context, m_material.get(), m_drawer, vertex[i].get());
	}
}

void MeshRender::render(Camera* camera) {
	if (m_pipelines.size() == 0 && m_vertex.size() > 0) {
		initPipeline(m_vertex);
	}
	if (gameObject()->getLayer() & camera->getCullMask() == 0) {
		return;
	}

	if (m_drawer == camera->getRenderPass()) {
		m_cameraOp(this, camera);
		for (auto& pipeline : m_pipelines) {
			pipeline->IA();
			pipeline->VS();
			pipeline->Rasterizer();
			pipeline->PS();
			pipeline->OM();
			pipeline->DrawIndex();
		}
	}
	
}

void MeshRender::setMaterial(std::shared_ptr<Material> material) {
	m_material = material;
}


void MeshRender::setVertex(std::shared_ptr<AnyVertexBuffer> vertex) {
	m_vertex.push_back(vertex);
}

