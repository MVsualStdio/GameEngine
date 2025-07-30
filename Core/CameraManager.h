#pragma once

#include <unordered_map>
#include "Camera.h"

class CameraManager {
public:
	using Manager = std::unordered_map<int, std::unordered_map<int, std::shared_ptr<ICamera>>>;
	CameraManager() = default;
	~CameraManager() = default;
	void addCamera(int group, int index, std::shared_ptr<ICamera> camera);
	std::unordered_map<int, std::shared_ptr<ICamera>> getCameraGroup(int group);
	ICamera* getCamera(int group, int index);
	const Manager& getCameraList();

	void setCurCameraGroup(int group);
	int getCurCameraGroupIndex();
	std::unordered_map<int, std::shared_ptr<ICamera>> getCurCameraGroup();
	ICamera* getCurCamera();

private:
	Manager m_camerList;
	int m_curCamerGroup = 0;
	int m_curCamerIndex = 0;
};