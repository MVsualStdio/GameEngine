#pragma once
#include "../Core/Component/GameObject.h"
#include "../Core/IDrawer.h"
#include "../Core/Component/Transform.h"
#include "XComponent/XLightCpt.h"

class XPlane : public GameObject {
public:
	XPlane();
	~XPlane();
	void init(IDrawer* draw, D3D11Context* context, Texture2D* texture, std::shared_ptr<AnyVertexBuffer> vertex);
private:
	Transform* m_transform;
	MeshRender* m_render;
	Texture2D* m_texture;
	XLightCpt* m_light;
};