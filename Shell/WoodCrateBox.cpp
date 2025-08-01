#include "WoodCrateBox.h"
#include "../Core/DDSTextureLoader11.h"
#include "../Core/MeshFilter.h"

WoodCrateBox::WoodCrateBox(IDrawer* drawer, D3D11Context* context, Scene* scene, Texture2D& texture)
	: MeshRender(drawer, context)
	, m_texture(texture)
	, m_scene(scene){

	ID3D11ShaderResourceView* textureView;
	DirectX::CreateDDSTextureFromFile(context->m_Device.Get(), L"E:/LearnSomething/RTTR/HLSL/WoodCrate.dds", nullptr, &textureView);
	Texture2D textureDDS(context, textureView);

	Material* material = new Material(context);
	material->setVSShader(L"E:/LearnSomething/RTTR/HLSL/woodCrateBoxLight.hlsli");
	material->setPSShader(L"E:/LearnSomething/RTTR/HLSL/woodCrateBoxLight.hlsli");
	Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
	
	//world(3, 2) = 30;
	material->getVSShader()->setUniform("g_World", world);
	material->getVSShader()->setUniform("g_WorldInvTranspose", world);

	material->getPSShader()->setUniform("lightPosition", Eigen::Vector3f{ 40.0f,40.0f,-40.0f });
	material->getPSShader()->setUniform("lightColor", Eigen::Vector3f{ 1.0f,1.0f,1.0f });

	material->getPSShader()->setTexture(0, textureDDS);
	material->getPSShader()->setTexture(1, m_texture);
	this->setMaterial(std::shared_ptr<Material>(material));
	this->setVertex(Geometry::CreateCube(10.0f, 10.0f, 10.0f));
}

void WoodCrateBox::tick(double dt) {
	//float RotationSpeed = 1.0f / 1000.0f;
	//m_time += RotationSpeed * dt;
	//Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
	//world.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_time, Eigen::Vector3f::UnitX()).toRotationMatrix();
	//getMaterial()->getVSShader()->setUniform("g_World", world);
	//Eigen::Matrix4f norworld = world.inverse().transpose();
	//getMaterial()->getVSShader()->setUniform("g_WorldInvTranspose", norworld);
}

void WoodCrateBox::cameraChange(ICamera* camera) {
	getMaterial()->getVSShader()->setUniform("g_View", camera->view());
	getMaterial()->getVSShader()->setUniform("g_Proj", camera->projection());
	getMaterial()->getPSShader()->setUniform("eyePosW", camera->position());
}