#include <rttr/registration>
#include "XWoodCrateBox.h"
#include "../Core/DDSTextureLoader11.h"
#include "../Core/MeshFilter.h"

#include "Eigen/Dense"
#include "../Core/Component/Component.h"
#include "../Core/VertexLayout.h"
#include "../Core/TextureManager.h"
#include "../Core/FileSystem.h"

XWoodCrateBox::XWoodCrateBox(IDrawer* drawer, D3D11Context* context, Texture2D* texture)
	: GameObject("XWoodCrateBox") {

	m_render = dynamic_cast<MeshRender*>(addOnlyComponent("MeshRender"));
	m_transform = dynamic_cast<Transform*>(addOnlyComponent("Transform"));
	m_light = dynamic_cast<XLightCpt*>(addOnlyComponent("XLight"));

	m_render->init(drawer, context);

	std::string texturePath = "D:/work/GameEngine/HLSL/WoodCrate.dds";
	TextureManager::instance()->addTexture(texturePath);

	Material* material = new Material(context);
	material->setVSShader(FileSystem::HLSLWPath("/woodCrateBoxLight.hlsli").data());
	material->setPSShader(FileSystem::HLSLWPath("/woodCrateBoxLight.hlsli").data());

	Eigen::Matrix4f world = m_transform->getMatrix();

	material->getVSShader()->setUniform("g_World", world);
	Eigen::Matrix4f worldInv = world.inverse().transpose();
	material->getVSShader()->setUniform("g_WorldInvTranspose", worldInv);
	material->getPSShader()->setUniform("lightPosition", m_light->getPosition());

	Eigen::Vector3f color = Eigen::Vector3f{ m_light->getLightColor().x(),m_light->getLightColor().y(),m_light->getLightColor().z() };
	material->getPSShader()->setUniform("lightColor", color);

	material->getPSShader()->setTexture(0, TextureManager::instance()->getTexture(texturePath, context));
	material->getPSShader()->setTexture(1, texture);

	m_render->setMaterial(std::shared_ptr<Material>(material));

	m_render->setVertex(Geometry::CreateCube(2.0f, 2.0f, 2.0f));

	m_render->cameraRender([](MeshRender* render, Camera* camera)->void {
		render->getMaterial()->getVSShader()->setUniform("g_View", camera->view());
		render->getMaterial()->getVSShader()->setUniform("g_Proj", camera->projection());

		Transform* trans = dynamic_cast<Transform*>(camera->gameObject()->getComponent("Transform"));
		render->getMaterial()->getPSShader()->setUniform("eyePosW", trans->getPosition());
	});
}


void XWoodCrateBox::update(double dt) {
	//float RotationSpeed = 1.0f / 1000.0f;
	//m_time += RotationSpeed * dt;
	//Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
	//world.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_time, Eigen::Vector3f::UnitX()).toRotationMatrix();
	//Eigen::Matrix4f norworld = world.inverse().transpose();
	//m_render->getMaterial()->getVSShader()->setUniform("g_WorldInvTranspose", norworld);
	//m_render->getMaterial()->getVSShader()->setUniform("g_World", world);
}