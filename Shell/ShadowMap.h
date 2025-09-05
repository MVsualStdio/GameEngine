#pragma once

#include "../Core/Component/GameObject.h"
#include "../Core/Component/Transform.h"
#include "../Core/MeshRender.h"
#include "../Core/Camera.h"
#include "../Core/RenderPass.h"
#include "XComponent/XLightCpt.h"
#include <vector>
#include "../Core/VertexLayout.h"
#include "../Core/Material.h"

class ShadowMap : GameObject{
public:
	ShadowMap(D3D11Context* context);

	void addShadowMap(GameObject* meshRender);

	Texture2D* getResult();
	Depth2D* getDepth();
	RenderPass* getPass() { return m_pass; }
	~ShadowMap();

	Camera* getCamera() { return m_camera; }

private:
	XLightCpt* m_light;
	Transform* m_transform;
	std::vector<MeshRender*> m_renderObjects;
	Camera* m_camera;
	RenderPass* m_pass;
	MeshRender* m_shadowRender;
	D3D11Context* m_context;
};

