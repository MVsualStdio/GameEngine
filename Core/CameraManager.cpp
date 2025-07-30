#include "CameraManager.h"


void CameraManager::addCamera(int group, int index, std::shared_ptr<ICamera> camera) {
	m_camerList[group][index] = camera;
}

const CameraManager::Manager& CameraManager::getCameraList() {
	return m_camerList;
}

std::unordered_map<int, std::shared_ptr<ICamera>> CameraManager::getCameraGroup(int group) {
	if (m_camerList.find(group) != m_camerList.end()) {
		return m_camerList[group];
	}
	return std::unordered_map<int, std::shared_ptr<ICamera>>();
}

ICamera* CameraManager::getCamera(int group, int index) {
	if (m_camerList.find(group) != m_camerList.end()) {
		if (m_camerList[group].find(index) != m_camerList[group].end()) {
			return m_camerList[group][index].get();
		}
	}
	return nullptr;
}

std::unordered_map<int, std::shared_ptr<ICamera>> CameraManager::getCurCameraGroup() {
	return getCameraGroup(m_curCamerGroup);
}

ICamera* CameraManager::getCurCamera() {
	return getCamera(m_curCamerGroup,m_curCamerIndex);
}

void CameraManager::setCurCameraGroup(int group) {
	m_curCamerGroup = group;
}

int CameraManager::getCurCameraGroupIndex() {
	return m_curCamerGroup;
}