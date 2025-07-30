#pragma once

#include <vector>
#include "IDrawer.h"
#include "D3D11Context.h"
#include <memory>
#include "CameraManager.h"

class DrawMangerBase {
public:
	DrawMangerBase();
	virtual ~DrawMangerBase();
	void present(double dt);
	void init(HWND winID, uint32_t width, uint32_t height);

	void addCamera(IDrawer* drawer, int group, int index, std::shared_ptr<ICamera> camera);
	ICamera* getCamera(IDrawer* drawer, int group, int index);

	std::unordered_map<int, std::shared_ptr<ICamera>> getCameraGroup(IDrawer* drawer, int group);
	std::unordered_map<int, std::shared_ptr<ICamera>> getCurCameraGroup(IDrawer* drawer);
	void setCurCameraManager(IDrawer* drawer, int group);
	int getCurCameraGroupIndex(IDrawer* drawer);

	void moveCamera(IDrawer* drawer, int group, int index, float x, float y, float z);
	void forwardCamera(IDrawer* drawer, int group, int index, float step);

	void updateCamera();

	int getDrawSize();
	IDrawer* getDrawer(int index);

	IDrawer* getScreenDrawer();

protected:
	virtual void prepare() = 0;
	virtual void initCompent() = 0;

protected:
	std::vector<IDrawer*> m_drawList;
	std::shared_ptr<D3D11Context> m_context;
	std::unordered_map<IDrawer*,CameraManager> m_cameraManager;
	int m_mainCameraIndex;
	
};


