#include "ShadowMap.h"
#include "../Core/IDrawer.h"

ShadowMap::ShadowMap(D3D11Context* context)
	: m_pass(new RenderPass(context))
	, m_context(context) {

	m_pass->setRenderOrder(1);
	m_transform = dynamic_cast<Transform*>(addOnlyComponent("Transform"));
	m_camera = dynamic_cast<Camera*>(addOnlyComponent("Camera"));
	m_light = dynamic_cast<XLightCpt*>(addOnlyComponent("XLight"));

	m_transform->setPosition(m_light->getPosition());
	m_camera->setOrtho(m_light->getLookAt(), 40, 40);
	m_camera->setRenderPass(m_pass);
}

void ShadowMap::addShadowMap(GameObject* meshRender) {
	std::vector<Component*> renders = meshRender->getComponents("MeshRender");
	for (auto component : renders) {
		MeshRender* render = dynamic_cast<MeshRender*>(component);
		if (render) {
			render->getMaterial()->getPSShader()->setEffectType(EffectType::None);
			render->addPass(m_pass);
		}
	}
}

Texture2D* ShadowMap::getResult() {
	return m_pass->getRenderTarget();
}

Depth2D* ShadowMap::getDepth() {
	return m_pass->getDepthStencil();
}


ShadowMap::~ShadowMap() {

}
