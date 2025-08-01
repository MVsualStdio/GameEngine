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
		auto curCameraGroup = getCurCameraGroup(drawer);
		drawer->clear();
		for (auto& [index, camera] : curCameraGroup) {
			drawer->updateCamera(camera.get());
			drawer->onDraw(dt);
		}
		drawer->present();
	}

}

void DrawMangerBase::init(HWND winID, uint32_t width, uint32_t height) {
	m_context = std::make_shared<D3D11Context>(winID, width, height);
	initCompent();
	prepare();
}

void DrawMangerBase::addCamera(IDrawer* drawer, int group, int index, std::shared_ptr<ICamera> camera) {
	if (drawer != nullptr) {
		m_cameraManager[drawer].addCamera(group, index, camera);
	}
}

ICamera* DrawMangerBase::getCamera(IDrawer* drawer, int group, int index) {
	if (drawer != nullptr && m_cameraManager.find(drawer) != m_cameraManager.end()) {
		return m_cameraManager[drawer].getCamera(group, index);
	}
	return nullptr;
}

std::unordered_map<int, std::shared_ptr<ICamera>> DrawMangerBase::getCameraGroup(IDrawer* drawer, int group) {
	if (drawer != nullptr && m_cameraManager.find(drawer) != m_cameraManager.end()) {
		return m_cameraManager[drawer].getCameraGroup(group);
	}
	return std::unordered_map<int, std::shared_ptr<ICamera>>();
}

std::unordered_map<int, std::shared_ptr<ICamera>> DrawMangerBase::getCurCameraGroup(IDrawer* drawer) {
	if (drawer != nullptr && m_cameraManager.find(drawer) != m_cameraManager.end()) {
		return m_cameraManager[drawer].getCurCameraGroup();
	}
	return std::unordered_map<int, std::shared_ptr<ICamera>>();
}

int DrawMangerBase::getCurCameraGroupIndex(IDrawer* drawer) {
	if (drawer != nullptr && m_cameraManager.find(drawer) != m_cameraManager.end()) {
		return m_cameraManager[drawer].getCurCameraGroupIndex();
	}
	return 0;
}

void DrawMangerBase::setCurCameraManager(IDrawer* drawer, int group) {
	if (drawer != nullptr && m_cameraManager.find(drawer) != m_cameraManager.end()) {
		m_cameraManager[drawer].setCurCameraGroup(group);
	}
}

void DrawMangerBase::updateCamera() {
	for (auto draw : m_drawList) {
		auto curCameraGroup = getCurCameraGroup(draw);
		for (auto& [index, camera] : curCameraGroup) {
			draw->updateCamera(camera.get());
		}
	}
}

void DrawMangerBase::moveCamera(IDrawer* drawer, int group, int index, float x, float y, float z) {
	getCamera(drawer, group, index)->move(Eigen::Vector3f{ x,y,z });
	updateCamera();
}

void DrawMangerBase::forwardCamera(IDrawer* drawer, int group, int index, float step) {
	getCamera(drawer, group, index)->forward(step);
	updateCamera();
}


void DrawMangerBase::rightCamera(IDrawer* drawer, int group, int index, float step) {
	getCamera(drawer, group, index)->right(step);
	updateCamera();
}

void DrawMangerBase::rotateCamera(IDrawer* drawer, int group, int index, float yaw, float pitch) {
	getCamera(drawer, group, index)->rotate(yaw, pitch);
	updateCamera();
}

int DrawMangerBase::getDrawSize() {
	return m_drawList.size();
}

IDrawer* DrawMangerBase::getDrawer(int index) {
	return m_drawList[index];
}

IDrawer* DrawMangerBase::getScreenDrawer() {
	for (auto& drawer : m_drawList) {
		DrawScreen* drawScreen = dynamic_cast<DrawScreen*>(drawer);
		if (drawScreen) {
			return drawScreen;
		}
	}
	return nullptr;
}