#include <rttr/registration>
#include "XWoodCrateBox.h"
#include "../Core/DDSTextureLoader11.h"
#include "../Core/MeshFilter.h"

#include "Eigen/Dense"
#include "../Core/Component/Component.h"


XWoodCrateBox::XWoodCrateBox()
	: GameObject("XWoodCrateBox") {

}

void XWoodCrateBox::init(IDrawer* drawer, D3D11Context* context, Texture2D* texture) {
	m_texture = texture;

	Component* component = addComponent("MeshRender");
	m_render = dynamic_cast<MeshRender*>(component);
	m_render->init(drawer, context);

	ID3D11ShaderResourceView* textureView;
	DirectX::CreateDDSTextureFromFile(context->m_Device.Get(), L"D:/work/GameEngine/HLSL/WoodCrate.dds", nullptr, &textureView);
	Texture2D textureDDS(context, textureView);

	m_transform = dynamic_cast<Transform*>(addComponent("Transform"));

	Material* material = new Material(context);
	material->setVSShader(L"D:/work/GameEngine/HLSL/woodCrateBoxLight.hlsli");
	material->setPSShader(L"D:/work/GameEngine/HLSL/woodCrateBoxLight.hlsli");
	
	Eigen::Matrix4f world = m_transform->getMatrix();

	material->getVSShader()->setUniform("g_World", world);
	Eigen::Matrix4f worldInv = world.inverse().transpose();
	material->getVSShader()->setUniform("g_WorldInvTranspose", worldInv);
	material->getPSShader()->setUniform("lightPosition", Eigen::Vector3f{ 30.0f,30.0f,-40.0f });
	material->getPSShader()->setUniform("lightColor", Eigen::Vector3f{ 1.0f,1.0f,1.0f });

	material->getPSShader()->setTexture(0, textureDDS);
	material->getPSShader()->setTexture(1, *m_texture);
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