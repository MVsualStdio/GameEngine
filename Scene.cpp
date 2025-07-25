#include "Scene.h"
#include "DrawScreen.h"

#include "DDSTextureLoader11.h"
#include "MeshFilter.h"
#include <Eigen/Geometry>



DrawMangerTexture::DrawMangerTexture() {

}

void DrawMangerTexture::initCompent() {
	std::shared_ptr<ICamera> camera = std::make_shared<ProjectionCamera>(Eigen::Vector3f{ 0,0,-30 }, Eigen::Vector3f{ 0,0,0 }, m_context->AspectRatio());
	addMainCamera(0, camera);
}

//void DrawMangerTexture::initDrawScreen(IDrawer* draw) {
//
//	ID3D11ShaderResourceView* textureView;
//	DirectX::CreateDDSTextureFromFile(m_context->m_Device.Get(), L"E:/LearnSomething/RTTR/HLSL/WoodCrate.dds", nullptr, &textureView);
//	Texture2D textureDDS(m_context.get(), textureView);
//
//	ICamera* camera = getMainCamera();
//	std::shared_ptr<MeshRender> meshRender = std::make_shared<MeshRender>(draw, m_context.get());
//	Material* material = new Material(m_context.get());
//	material->setVSShader(L"E:/LearnSomething/RTTR/HLSL/baseVS.hlsli");
//	material->setPSShader(L"E:/LearnSomething/RTTR/HLSL/basePS.hlsli");
//
//	material->getVSShader()->setUniform("g_World", Eigen::Matrix4f::Identity());
//	material->getVSShader()->setUniform("g_View", camera->view());
//	material->getVSShader()->setUniform("g_Proj", camera->projection());
//
//	material->getPSShader()->setTexture(0, textureDDS);
//	meshRender->setMaterial(std::shared_ptr<Material>(material));
//	meshRender->setVertex(Geometry::CreateCube(10.0f, 10.0f, 10.0f));
//	
//	meshRender->setUpdateFun([&](MeshRender* ptrThis, double dt) {
//		m_time = m_time + 0.001;
//		Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
//		world.block<3,3>(0,0) = Eigen::AngleAxisf(m_time, Eigen::Vector3f::UnitZ()).toRotationMatrix();
//		ptrThis->getMaterial()->getVSShader()->setUniform("g_World", world);
//	});
//	
//	draw->addItem(meshRender);

//}

void DrawMangerTexture::initDrawScreen(IDrawer* draw) {

	std::shared_ptr<MeshRender> box = std::make_shared<WoodCrateBox>(draw, m_context.get(), getMainCamera());

	draw->addItem(box);
}


void DrawMangerTexture::initDrawTexture(IDrawer* draw) {
	ID3D11ShaderResourceView* textureView;
	DirectX::CreateDDSTextureFromFile(m_context->m_Device.Get(), L"E:/LearnSomething/RTTR/HLSL/flare.dds", nullptr, &textureView);
	Texture2D textureDDS(m_context.get(), textureView);

	std::unique_ptr<MeshRender> meshRender = std::make_unique<MeshRender>(draw, m_context.get());
	Material* material = new Material(m_context.get());
	material->setVSShader(L"E:/LearnSomething/RTTR/HLSL/baseVS.hlsli");
	material->setPSShader(L"E:/LearnSomething/RTTR/HLSL/basePS.hlsli");

	material->getVSShader()->setUniform("g_World", Eigen::Matrix4f::Identity());
	material->getVSShader()->setUniform("g_View", Eigen::Matrix4f::Identity());
	material->getVSShader()->setUniform("g_Proj", Eigen::Matrix4f::Identity());

	material->getPSShader()->setTexture(0, textureDDS);

	meshRender->setMaterial(std::shared_ptr<Material>(material));
	Eigen::Vector3f points[3] = { { 0.0f, 0.5f, 0.5f },{ 0.5f, -0.5f, 0.5f },{ -0.5f, -0.5f, 0.5f } };
	meshRender->setVertex(Geometry::CreateTrangle(points));
	draw->addItem(std::move(meshRender));
}

void DrawMangerTexture::initDrawer() {
	//texture = std::make_shared<Texture2D>(m_context.get(), m_context->width(), m_context->height());
	//DrawTexture* drawTexture = new DrawTexture(texture.get());
	//drawTexture->init(m_context.get());
	//drawTexture->initDrawFunction([this](IDrawer* draw) {
	//	initDrawTexture(draw);
	//	});
	//m_drawList.push_back(drawTexture);

	DrawScreen* drawScreen = new DrawScreen();
	drawScreen->init(m_context.get());
	drawScreen->initDrawFunction([this](IDrawer* draw) {
		initDrawScreen(draw);
		});

	m_drawList.push_back(drawScreen);

}


WoodCrateBox::WoodCrateBox(IDrawer* drawer, D3D11Context* context, ICamera* camera)
	: MeshRender(drawer, context) {

	m_update = std::bind(&WoodCrateBox::tick, this, std::placeholders::_2);

	ID3D11ShaderResourceView* textureView;
	DirectX::CreateDDSTextureFromFile(context->m_Device.Get(), L"E:/LearnSomething/RTTR/HLSL/WoodCrate.dds", nullptr, &textureView);
	Texture2D textureDDS(context, textureView);

	Material* material = new Material(context);
	material->setVSShader(L"E:/LearnSomething/RTTR/HLSL/baseVS.hlsli");
	material->setPSShader(L"E:/LearnSomething/RTTR/HLSL/basePS.hlsli");

	material->getVSShader()->setUniform("g_World", Eigen::Matrix4f::Identity());
	material->getVSShader()->setUniform("g_View", camera->view());
	material->getVSShader()->setUniform("g_Proj", camera->projection());

	material->getPSShader()->setTexture(0, textureDDS);
	this->setMaterial(std::shared_ptr<Material>(material));
	this->setVertex(Geometry::CreateCube(10.0f, 10.0f, 10.0f));

}

void WoodCrateBox::tick(double dt) {

	float RotationSpeed = 1.0f / 1000.0f;
	m_time += RotationSpeed * dt;
	Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
	world.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_time, Eigen::Vector3f::UnitZ()).toRotationMatrix();
	getMaterial()->getVSShader()->setUniform("g_World", world);
	
}