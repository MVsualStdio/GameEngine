#include "DrawManger.h"
#include "DrawScreen.h"
#include "MeshRender.h"

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

void DrawMangerBase::initDrawer() {
	DrawScreen* drawScreen = new DrawScreen();
	drawScreen->init(m_context.get());
	m_drawList.push_back(drawScreen);
}

void DrawMangerBase::initMeshRender() {
	for (auto draw : m_drawList) {
		initMeshRenderItem(draw);
	}
}

void DrawMangerBase::initMeshRenderItem(IDrawer* draw) {
	MeshRender* meshRender = new MeshRender(draw, m_context.get());
	Material* material = new Material(m_context.get());
	material->setVSShader(L"E:/LearnSomething/RTTR/HLSL/baseVS.hlsli");
	material->setPSShader(L"E:/LearnSomething/RTTR/HLSL/basePS.hlsli");
	meshRender->setMaterial(material);
	draw->addItem(meshRender);
}