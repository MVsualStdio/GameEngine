#pragma once

#include "../Core/Component/GameObject.h"
#include "XComponent/XLightCpt.h"
#include "../Core/IDrawer.h"

class XLight : public GameObject {
public:
	XLight();
	~XLight() = default;
	void update(double dt);
	void init(IDrawer* drawer, D3D11Context* context);
private:
	Transform* m_transform;
	MeshRender* m_render;
	XLightCpt* m_lightCpt;
};