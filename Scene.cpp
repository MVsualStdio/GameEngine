#include "Scene.h"
#include "DrawScreen.h"

#include "DDSTextureLoader11.h"
#include "MeshFilter.h"
#include <Eigen/Geometry>
#include "GameObjBox.h"

Scene::Scene(D3D11Context* context, IDrawer* drawer)
	: m_context(context)
	, m_drawer(drawer) {

}

Scene::~Scene() {

}

void Scene::render(double dt) {
	for (const auto& [name, mesh] : m_renderItems) {
		mesh->render(dt);
	}
}

void Scene::updateCamera(ICamera* camera) {
	for (const auto& [name, mesh] : m_renderItems) {
		mesh->updateCamera(camera);
	}
}

void Scene::addMeshRender(const std::string& name, std::shared_ptr<MeshRender> mesh) {
	m_renderItems[name] = mesh;
}

std::shared_ptr<MeshRender> Scene::getRender(const std::string& name) {
	if (m_renderItems.find(name) != m_renderItems.end()) {
		return m_renderItems[name];
	}
	return nullptr;
}

const Scene::RenderItem& Scene::getAllRenderItems() {
	return m_renderItems;
}

void Scene::setDrawer(IDrawer* drawer) {
	m_drawer = drawer;
}




DrawMangerTexture::DrawMangerTexture() {

}

void DrawMangerTexture::initCompent() {
	std::shared_ptr<ICamera> camera = std::make_shared<ProjectionCamera>(Eigen::Vector3f{ 0,0,-30 }, Eigen::Vector3f{ 0,0,0 }, m_context->AspectRatio());
	addMainCamera(0, camera);
}

void DrawMangerTexture::initDrawScreen(IDrawer* draw) {

	ICamera* camera = getMainCamera();
	std::shared_ptr<MeshRender> meshRender = std::make_shared<MeshRender>(draw, m_context.get());
	Material* material = new Material(m_context.get());
	material->setVSShader(L"E:/LearnSomething/RTTR/HLSL/baseVS.hlsli");
	material->setPSShader(L"E:/LearnSomething/RTTR/HLSL/basePS.hlsli");

	material->getVSShader()->setUniform("g_World", Eigen::Matrix4f::Identity());
	material->getVSShader()->setUniform("g_View", camera->view());
	material->getVSShader()->setUniform("g_Proj", camera->projection());

	RenderPass* renderPass = dynamic_cast<RenderPass*>(m_drawList[0]);
	if (renderPass) {
		material->getPSShader()->setTexture(0, *renderPass->getResult());
	}
	
	meshRender->setMaterial(std::shared_ptr<Material>(material));
	meshRender->setVertex(Geometry::CreateCube(10.0f, 10.0f, 10.0f));
	
	meshRender->setRenderCamera(camera);

	meshRender->setUpdateFun([&](MeshRender* render, double dt) {
		m_time = m_time + 0.001;
		Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
		world.block<3,3>(0,0) = Eigen::AngleAxisf(m_time, Eigen::Vector3f::UnitZ()).toRotationMatrix();
		render->getMaterial()->getVSShader()->setUniform("g_World", world);
	});

	meshRender->setUpdateUniform([](MeshRender* render) {
		render->getMaterial()->getVSShader()->setUniform("g_View", render->getRenderCamera()->view());
		render->getMaterial()->getVSShader()->setUniform("g_Proj", render->getRenderCamera()->projection());
	});

	draw->addItem(meshRender);
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

void DrawMangerTexture::prepare() {

	RenderPass* drawTexture = new RenderPass(m_context.get());
	initDrawTexture(drawTexture);
	m_drawList.push_back(drawTexture);

	DrawScreen* drawScreen = new DrawScreen(m_context.get());
	initDrawScreen(drawScreen);
	m_drawList.push_back(drawScreen);

}

