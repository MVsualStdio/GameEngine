#pragma once

#include "../Core/Component/GameObject.h"
#include "../Core/Component/Transform.h"
#include "../Core/IDrawer.h"
#include "../Core/MeshRender.h"
#include "XComponent/XLightCpt.h"
#include "../Core/MeshFilter.h"
#include "../Core/Animation/KeyAnimation.h"

class XCarAnimation : public GameObject {
public:
	XCarAnimation();
	~XCarAnimation() = default;
	void init(IDrawer* draw, D3D11Context* context);
	void update(double dt) override;
private:
	double m_time = 0.0;
	Transform* m_transform;
	std::unordered_map<std::string, MeshRender*> m_render;
	XLightCpt* m_light;
	Animation* m_anim;
	std::unordered_map<std::string, NodeMesh> m_nodeMesh;
};
