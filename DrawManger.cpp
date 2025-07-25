#include "DrawManger.h"
#include "DrawScreen.h"
#include "MeshRender.h"
#include "DDSTextureLoader11.h"
#include "MeshFilter.h"

DrawMangerBase::DrawMangerBase()
	: m_mainCameraIndex(-1) {
}

DrawMangerBase::~DrawMangerBase() {
	for (IDrawer* drawer : m_drawList) {
		delete drawer;
	}
}

void DrawMangerBase::present(double dt) {
	for (IDrawer* drawer : m_drawList) {
		drawer->present(dt);
	}
}

void DrawMangerBase::init(HWND winID, uint32_t width, uint32_t height) {
	m_context = std::make_shared<D3D11Context>(winID, width, height);
	initCompent();
	initDrawer();
	initMeshRender();
}

void DrawMangerBase::initMeshRender() {
	for (auto draw : m_drawList) {
		draw->initRender();
	}
}

void DrawMangerBase::addMainCamera(int index, std::shared_ptr<ICamera> camera) {
	addOtherCamera(index, camera);
	m_mainCameraIndex = index;
}
void DrawMangerBase::addOtherCamera(int index, std::shared_ptr<ICamera> camera) {
	if (m_cameraList.find(index) == m_cameraList.end()) {
		m_cameraList.emplace(std::pair{ index,camera });
	}
	else {
		m_cameraList[index] = camera;
	}
}

void DrawMangerBase::setMainCamera(int index) {
	if (m_cameraList.find(index) != m_cameraList.end()) {
		m_mainCameraIndex = index;
	}
}
ICamera* DrawMangerBase::getCamera(int index) {
	if (m_cameraList.find(index) != m_cameraList.end()) {
		return m_cameraList[index].get();
	}
	return nullptr;
}

ICamera* DrawMangerBase::getMainCamera() {
	return getCamera(m_mainCameraIndex);
}