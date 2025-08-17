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

void XPlane::init(IDrawer* drawer, D3D11Context* context, Texture2D* texture) {
	m_render = dynamic_cast<MeshRender*>(addComponent("MeshRender"));
	m_render->init(drawer, context);

	m_transform = dynamic_cast<Transform*>(addComponent("Transform"));
	m_transform->setPosition(Eigen::Vector3f(0.0f, -1.0f, 0.0f));
	Eigen::Matrix4f world = m_transform->getMatrix();

	Material* material = new Material(context);
	material->setVSShader(L"D:/work/GameEngine/HLSL/floor.hlsli");
	material->setPSShader(L"D:/work/GameEngine/HLSL/floor.hlsli");
	
	material->getVSShader()->setUniform("g_World", world);
	Eigen::Matrix4f worldInv = world.inverse().transpose();
	material->getVSShader()->setUniform("g_WorldInvTranspose", worldInv);
	material->getPSShader()->setUniform("lightPosition", Eigen::Vector3f{ 30.0f,30.0f,-40.0f });
	material->getPSShader()->setUniform("lightColor", Eigen::Vector3f{ 1.0f,1.0f,1.0f });

	material->getPSShader()->setTexture(0, *texture);
	m_render->setMaterial(std::shared_ptr<Material>(material));
	m_render->setVertex(Geometry::CreatePlane(20.0f, 20.0f, 5.0f, 5.0f));

	m_render->cameraRender([](MeshRender* render, Camera* camera)->void {
		render->getMaterial()->getVSShader()->setUniform("g_View", camera->view());
		render->getMaterial()->getVSShader()->setUniform("g_Proj", camera->projection());

		Transform* trans = dynamic_cast<Transform*>(camera->gameObject()->getComponent("Transform"));
		render->getMaterial()->getPSShader()->setUniform("eyePosW", trans->getPosition());
	});
}