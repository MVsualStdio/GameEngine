#include "XLight.h"
#include "XPlane.h"
#include "../Core/MeshFilter.h"
#include "../Core/MeshRender.h"
#include "Eigen/Dense"
#include "../Core/Component/Component.h"

XLight::XLight() 
	:GameObject("light") {

}

void XLight::update(double dt) {

}

void XLight::init(IDrawer* drawer, D3D11Context* context) {
	m_render = dynamic_cast<MeshRender*>(addOnlyComponent("MeshRender"));
	m_transform = dynamic_cast<Transform*>(addOnlyComponent("Transform"));
	m_lightCpt = dynamic_cast<XLightCpt*>(addOnlyComponent("XLight"));

	m_render->init(drawer, context);

	m_transform->setPosition(m_lightCpt->getPosition());

	Eigen::Matrix4f world = m_transform->getMatrix();

	Material* material = new Material(context);
	material->setVSShader(L"D:/work/GameEngine/HLSL/light.hlsli");
	material->setPSShader(L"D:/work/GameEngine/HLSL/light.hlsli");
	material->getVSShader()->setUniform("g_World", world);

	m_render->setMaterial(std::shared_ptr<Material>(material));
	//Eigen::Vector4f{1.0f,0.0f,0.0f,1.0f}
	m_render->setVertex(Geometry::CreateCube(1.0f, 1.0f, 1.0f, m_lightCpt->getLightColor()));

	m_render->cameraRender([](MeshRender* render, Camera* camera)->void {
		render->getMaterial()->getVSShader()->setUniform("g_View", camera->view());
		render->getMaterial()->getVSShader()->setUniform("g_Proj", camera->projection());

		Transform* trans = dynamic_cast<Transform*>(camera->gameObject()->getComponent("Transform"));
	});
}