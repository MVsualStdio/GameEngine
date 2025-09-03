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
	m_drawer.push_back(drawer);
}

void MeshRender::cameraRender(MeshRender::CameraChangeFunction op) {
	m_cameraOp = op;
}

void MeshRender::initPipeline(std::vector<std::shared_ptr<AnyVertexBuffer>> vertex) {
	size_t vertexSize = vertex.size();
	for (int i = 0; i < vertexSize; ++i) {
		for (int j = 0; j < m_drawer.size(); ++j) {
			m_pipelines[m_drawer[j]] = std::make_shared<Pipeline>(m_context, m_material.get(), m_drawer[j], vertex[i].get());
		}
	}
}

void MeshRender::render(Camera* camera, bool forceRender) {
	if (m_vertex.size() > 0) {
		if (!m_init || m_pipelines.size() == 0) {
			initPipeline(m_vertex);
			m_init = true;
		}
	}

	if (gameObject()->getLayer() & camera->getCullMask() == 0) {
		return;
	}

	bool existPass = false;

	for (auto drawer : m_drawer) {
		existPass |= drawer == camera->getRenderPass();
	}
	if (forceRender || existPass) {
		m_cameraOp(this, camera);
		IDrawer* pass = camera->getRenderPass();
		if (m_pipelines.find(pass) != m_pipelines.end()) {
			std::shared_ptr<Pipeline> pipeline = m_pipelines[pass];
			pipeline->clear();
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

void MeshRender::addPass(IDrawer* drawer) {
	m_drawer.push_back(drawer);
	m_init = false;
}

void MeshRender::setVertex(std::shared_ptr<AnyVertexBuffer> vertex) {
	m_vertex.push_back(vertex);
}

void MeshRender::setVertex(std::shared_ptr<AnyVertexBuffer> vertex, int index) {
	m_vertex[index] = vertex;
	m_init = false;
}

void MeshRender::resetVertex() {
	m_vertex.clear();
	m_init = false;
}