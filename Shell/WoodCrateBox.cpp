#include "WoodCrateBox.h"
#include "../Core/DDSTextureLoader11.h"
#include "../Core/MeshFilter.h"

WoodCrateBox::WoodCrateBox(IDrawer* drawer, D3D11Context* context, ICamera* camera, Texture2D& texture)
	: MeshRender(drawer, context)
	, m_texture(texture){

	setRenderCamera(camera);
	ID3D11ShaderResourceView* textureView;
	DirectX::CreateDDSTextureFromFile(context->m_Device.Get(), L"E:/LearnSomething/RTTR/HLSL/WoodCrate.dds", nullptr, &textureView);
	Texture2D textureDDS(context, textureView);

	Material* material = new Material(context);
	material->setVSShader(L"E:/LearnSomething/RTTR/HLSL/woodCrateBox.hlsli");
	material->setPSShader(L"E:/LearnSomething/RTTR/HLSL/woodCrateBox.hlsli");
	Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
	//world(3, 2) = 30;
	material->getVSShader()->setUniform("g_World", world);
	material->getVSShader()->setUniform("g_View", camera->view());
	material->getVSShader()->setUniform("g_Proj", camera->projection());

	material->getPSShader()->setTexture(0, textureDDS);
	material->getPSShader()->setTexture(1, m_texture);
	this->setMaterial(std::shared_ptr<Material>(material));
	this->setVertex(Geometry::CreateCube(10.0f, 10.0f, 10.0f));
}

void WoodCrateBox::tick(double dt) {
	float RotationSpeed = 1.0f / 1000.0f;
	m_time += RotationSpeed * dt;
	Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
	world.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_time, Eigen::Vector3f::UnitX()).toRotationMatrix();
	getMaterial()->getVSShader()->setUniform("g_World", world);
}

void WoodCrateBox::cameraChange() {
	getMaterial()->getVSShader()->setUniform("g_View", m_camera->view());
	getMaterial()->getVSShader()->setUniform("g_Proj", m_camera->projection());
}