#include "XPlane.h"
#include "../Core/MeshFilter.h"
#include "../Core/MeshRender.h"
#include "Eigen/Dense"
#include "../Core/Component/Component.h"

XPlane::XPlane()
	: GameObject(){

}

XPlane::~XPlane() {

}

void XPlane::init(IDrawer* drawer, D3D11Context* context, Texture2D* texture, std::shared_ptr<AnyVertexBuffer> vertex) {
	m_render = dynamic_cast<MeshRender*>(addOnlyComponent("MeshRender"));
	m_transform = dynamic_cast<Transform*>(addOnlyComponent("Transform"));
	m_light = dynamic_cast<XLightCpt*>(addOnlyComponent("XLight"));

	m_render->init(drawer, context);
	
	Eigen::Matrix4f world = m_transform->getMatrix();

	Material* material = new Material(context);
	material->setVSShader(L"D:/work/GameEngine/HLSL/floor.hlsli");
	material->setPSShader(L"D:/work/GameEngine/HLSL/floor.hlsli");
	
	material->getVSShader()->setUniform("g_World", world);
	Eigen::Matrix4f worldInv = world.inverse().transpose();
	material->getVSShader()->setUniform("g_WorldInvTranspose", worldInv);
	material->getPSShader()->setUniform("lightPosition", m_light->getPosition());

	Eigen::Vector3f color = Eigen::Vector3f{ m_light->getLightColor().x(),m_light->getLightColor().y(),m_light->getLightColor().z() };
	material->getPSShader()->setUniform("lightColor", color);

	material->getPSShader()->setTexture(0, *texture);
	m_render->setMaterial(std::shared_ptr<Material>(material));
	m_render->setVertex(vertex);

	m_render->cameraRender([](MeshRender* render, Camera* camera)->void {
		render->getMaterial()->getVSShader()->setUniform("g_View", camera->view());
		render->getMaterial()->getVSShader()->setUniform("g_Proj", camera->projection());

		Transform* trans = dynamic_cast<Transform*>(camera->gameObject()->getComponent("Transform"));
		render->getMaterial()->getPSShader()->setUniform("eyePosW", trans->getPosition());
	});
}