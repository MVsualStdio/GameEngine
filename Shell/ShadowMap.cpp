#include "ShadowMap.h"
#include "../Core/IDrawer.h"

ShadowMap::ShadowMap(D3D11Context* context)
	: m_pass(new RenderPass(context)) {

	m_transform = dynamic_cast<Transform*>(addOnlyComponent("Transform"));
	m_camera = dynamic_cast<Camera*>(addOnlyComponent("Camera"));
	m_light = dynamic_cast<XLightCpt*>(addOnlyComponent("XLight"));
	//m_shadowRender = dynamic_cast<MeshRender*>(addComponent("MeshRender"));

	m_transform->setPosition(m_light->getPosition());

	m_camera->setOrtho(m_light->getLookAt(), 40, 40);
	m_camera->setRenderPass(m_pass);

}

void ShadowMap::addShadowMap(GameObject* meshRender) {
	MeshRender* render = dynamic_cast<MeshRender*>(meshRender->getComponent("MeshRender"));
	if (render) {
		m_renderObjects.push_back(render);
		render->addPass(m_pass);
	}
}


Texture2D* ShadowMap::getResult() {
	return m_pass->getRenderTarget();
}

ShadowMap::~ShadowMap() {

}
