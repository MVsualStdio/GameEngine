#include "DrawScreenManager.h"
#include "../Core/DDSTextureLoader11.h"
#include <Eigen/Geometry>
#include "WoodCrateBox.h"
#include "../Core/MeshFilter.h"

DrawMangerScreen::DrawMangerScreen() {

}

void DrawMangerScreen::initScene() {
	std::shared_ptr<MeshRender> box = std::make_shared<WoodCrateBox>(m_drawScreen, m_context.get(), *m_Tranglepass->getResult());
	m_scene->addMeshRender("box", box);
}

void DrawMangerScreen::initTranglePass() {
	ID3D11ShaderResourceView* textureView;
	DirectX::CreateDDSTextureFromFile(m_context->m_Device.Get(), L"E:/LearnSomething/RTTR/HLSL/flare.dds", nullptr, &textureView);
	Texture2D textureDDS(m_context.get(), textureView);

	std::unique_ptr<MeshRender> trangle = std::make_unique<MeshRender>(m_Tranglepass, m_context.get());
	Material* material = new Material(m_context.get());
	material->setVSShader(L"E:/LearnSomething/RTTR/HLSL/baseVS.hlsli");
	material->setPSShader(L"E:/LearnSomething/RTTR/HLSL/basePS.hlsli");
	material->getVSShader()->setUniform("g_World", Eigen::Matrix4f::Identity());

	material->getPSShader()->setTexture(0, textureDDS);
	trangle->setMaterial(std::shared_ptr<Material>(material));
	Eigen::Vector3f points[3] = { { 0.0f, 0.5f, 0.5f },{ 0.5f, -0.5f, 0.5f },{ -0.5f, -0.5f, 0.5f } };
	trangle->setVertex(Geometry::CreateTrangle(points));

	trangle->setUpdateCamera([](MeshRender* render, const Eigen::Matrix4f& view, const Eigen::Matrix4f& projection) {
		render->getMaterial()->getVSShader()->setUniform("g_View", Eigen::Matrix4f::Identity());
		render->getMaterial()->getVSShader()->setUniform("g_Proj", Eigen::Matrix4f::Identity());
	});


	m_Tranglepass->addItem(std::move(trangle));
}

void DrawMangerScreen::initCompent() {
	std::shared_ptr<ICamera> camera = std::make_shared<ProjectionCamera>(Eigen::Vector3f{ 0,0,-30 }, Eigen::Vector3f{ 0,0,0 }, m_context->AspectRatio());
	std::shared_ptr<ICamera> camera2 = std::make_shared<ProjectionCamera>(Eigen::Vector3f{ 30,30,-30 }, Eigen::Vector3f{ 0,0,0 }, m_context->AspectRatio());
	std::shared_ptr<ICamera> nocamera = std::make_shared<NoCamera>();
	
	m_Tranglepass = new RenderPass(m_context.get());
	m_drawScreen = new DrawScreen(m_context.get());

	addCamera(m_drawScreen, 0, 0, camera);
	addCamera(m_drawScreen, 0, 1, camera2);
	addCamera(m_drawScreen, 1, 0, camera2);
	
	addCamera(m_Tranglepass, 0, 0, nocamera);

	m_drawList.push_back(m_Tranglepass);
	m_drawList.push_back(m_drawScreen);

	m_scene = std::make_shared<Scene>(m_context.get(), m_drawScreen);
	m_drawScreen->addItem(m_scene);
	
	initScene();
	initTranglePass();
}

void DrawMangerScreen::prepare() {
	
}