#pragma once

#include "../Core/Component/GameObject.h"
#include "../Core/Component/Transform.h"
#include "../Core/IDrawer.h"
#include "../Core/MeshRender.h"
#include "XComponent/XLightCpt.h"
#include "../Core/Animation/KeyAnimation.h"

class XWoodCrateBox : public GameObject {
public:
	XWoodCrateBox();
	~XWoodCrateBox() = default;
	void init(IDrawer* draw, D3D11Context* context, Texture2D* texture);
	void update(double dt) override;
private:
	double m_time = 0.0;
	Transform* m_transform;
	MeshRender* m_render;
	Texture2D* m_texture;
	XLightCpt* m_light;
	KeyAnimation* anim;
	std::vector<MeshInfo> mesh;

};
