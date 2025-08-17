#pragma once 

#include "../Core/Component/GameObject.h"
#include "../Core/IDrawer.h"
#include "../Core/Component/Transform.h"

class XCameraObject : public GameObject {
public:
	XCameraObject();
	~XCameraObject();
	void update(double dt);
	void init(IDrawer* draw, D3D11Context* context);
private:
	const double M_PI = 3.14159265358979323846;
	Camera* m_camera;
	Transform* m_cameraTransfrom;
};