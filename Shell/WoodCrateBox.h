#pragma once

#include "../Core/Component/Component.h"
#include "../Core/Component/Transform.h"
#include "../Core/IDrawer.h"
#include "../Core/MeshRender.h"

class WoodCrateBox : public Component {
public:
	WoodCrateBox();
	~WoodCrateBox() = default;
	void init(IDrawer* draw, D3D11Context* context, Texture2D* texture);
	void update(double dt)override;
private:
	const double M_PI = 3.14159265358979323846;
	double m_time = 0.0;
	Transform* m_transform;
	MeshRender* m_render;
	Transform* m_cameraTransfrom;
	Camera* m_camera;
	Texture2D* m_texture;
};
