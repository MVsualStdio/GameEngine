#pragma once

#include <vector>
#include "IDrawer.h"
#include "D3D11Context.h"
#include <memory>
#include "Camera.h"

class DrawMangerBase {
public:
	DrawMangerBase();
	virtual ~DrawMangerBase();
	void present(double dt);
	void init(HWND winID, uint32_t width, uint32_t height);

	void initMeshRender();
	void addMainCamera(int index, std::shared_ptr<ICamera> camera);
	void addOtherCamera(int index, std::shared_ptr<ICamera> camera);
	void setMainCamera(int index);
	ICamera* getCamera(int index);
	ICamera* getMainCamera();
	void moveMainCamera(float x, float y, float z);
	void updateCamera();

protected:
	virtual void initDrawer() = 0;
	virtual void initCompent() = 0;

protected:
	std::vector<IDrawer*> m_drawList;
	std::shared_ptr<D3D11Context> m_context;
	std::unordered_map<int, std::shared_ptr<ICamera>> m_cameraList;
	int m_mainCameraIndex;
	
};


