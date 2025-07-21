#include "DrawManger.h"
#include "DrawScreen.h"
#include "MeshRender.h"
#include "DDSTextureLoader11.h"

DrawMangerBase::DrawMangerBase() {

}

DrawMangerBase::~DrawMangerBase() {
	
}

void DrawMangerBase::present(double dt) {
	for (IDrawer* drawer : m_drawList) {
		drawer->present(dt);
	}
}

void DrawMangerBase::init(HWND winID, uint32_t width, uint32_t height) {
	m_context = std::make_shared<D3D11Context>(winID, width, height);
	initDrawer();
	initMeshRender();
}

void DrawMangerBase::initMeshRender() {
	for (auto draw : m_drawList) {
		draw->initRender();
	}
}


void DrawMangerTexture::initDrawScreen(IDrawer* draw) {
	std::unique_ptr<MeshRender> meshRender = std::make_unique<MeshRender>(draw, m_context.get());
	Material* material = new Material(m_context.get());
	material->setVSShader(L"E:/LearnSomething/RTTR/HLSL/baseVS.hlsli");
	material->setPSShader(L"E:/LearnSomething/RTTR/HLSL/basePS.hlsli");

	material->getVSShader()->setUniform("g_World", Eigen::Matrix4f::Identity());
	material->getVSShader()->setUniform("g_View", Eigen::Matrix4f::Identity());
	material->getVSShader()->setUniform("g_Proj", Eigen::Matrix4f::Identity());

	material->getPSShader()->setTexture(0, *texture);

	meshRender->setMaterial(std::shared_ptr<Material>(material));
	draw->addItem(std::move(meshRender));
}

void DrawMangerTexture::initDrawTexture(IDrawer* draw) {
	std::unique_ptr<MeshRender> meshRender = std::make_unique<MeshRender>(draw, m_context.get());
	Material* material = new Material(m_context.get());
	material->setVSShader(L"E:/LearnSomething/RTTR/HLSL/baseVS.hlsli");
	material->setPSShader(L"E:/LearnSomething/RTTR/HLSL/basePS.hlsli");

	material->getVSShader()->setUniform("g_World", Eigen::Matrix4f::Identity());
	material->getVSShader()->setUniform("g_View", Eigen::Matrix4f::Identity());
	material->getVSShader()->setUniform("g_Proj", Eigen::Matrix4f::Identity());

	ID3D11ShaderResourceView* textureView;
	DirectX::CreateDDSTextureFromFile(m_context->m_Device.Get(), L"E:/LearnSomething/RTTR/HLSL/flare.dds", nullptr, &textureView);
	Texture2D textureDDS(m_context.get(), textureView);
	material->getPSShader()->setTexture(0, textureDDS);
	
	meshRender->setMaterial(std::shared_ptr<Material>(material));
	draw->addItem(std::move(meshRender));
}

void DrawMangerTexture::initDrawer() {
	texture = new Texture2D(m_context.get(), m_context->width(), m_context->height());
	DrawTexture* drawTexture = new DrawTexture(texture);
	drawTexture->init(m_context.get());
	drawTexture->initDrawFunction([this](IDrawer* draw) {
		initDrawTexture(draw);
	});
	m_drawList.push_back(drawTexture);


	DrawScreen* drawScreen = new DrawScreen();
	drawScreen->init(m_context.get());
	drawScreen->initDrawFunction([this](IDrawer* draw) {
		initDrawScreen(draw);
	});
	m_drawList.push_back(drawScreen);

}