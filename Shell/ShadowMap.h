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

	template<class VertexType>
	void addShadowMap(GameObject* meshRender);

	Texture2D* getResult();
	~ShadowMap();

private:
	XLightCpt* m_light;
	Transform* m_transform;
	std::vector<MeshRender*> m_renderObjects;
	Camera* m_camera;
	RenderPass* m_pass;
	MeshRender* m_shadowRender;
	D3D11Context* m_context;
};

template<class VertexType>
void ShadowMap::addShadowMap(GameObject* meshRender) {
	MeshRender* render = dynamic_cast<MeshRender*>(meshRender->getComponent("MeshRender"));
	if (render) {
		MeshRender* newRender = dynamic_cast<MeshRender*>(addComponent("MeshRender"));
		newRender->init(m_pass, m_context);
		for (auto vertexs : render->getVertex()) {
			std::shared_ptr<VertexBuffer<VertexType>> vertexData = vertexs->vertex<VertexType>();
			std::shared_ptr<VertexBuffer<VertexPos>> pVertex = std::make_shared<VertexPosData>();
			int size = vertexData->vertices.size();
			pVertex->vertices.resize(size);
			for (int i = 0; i < size; ++i) {
				pVertex->vertices[i].pos = vertexData->vertices[i].pos;
			}
			size = vertexData->indices.size();
			pVertex->indices.resize(size);
			for (int i = 0; i < size; ++i) {
				pVertex->indices[i] = vertexData->indices[i];
			}
			newRender->setVertex(std::make_shared<AnyVertexBuffer>(pVertex));
		}

		Material* material = new Material(m_context);
		material->setVSShader(FileSystem::HLSLWPath("/Shadow.hlsli").data());
		material->setPSShader(FileSystem::HLSLWPath("/Shadow.hlsli").data());

		Transform* transform = dynamic_cast<Transform*>(render->gameObject()->getComponent("Transform"));
		material->getVSShader()->setUniform("g_World", transform->getMatrix());

		newRender->setMaterial(std::shared_ptr<Material>(material));

		newRender->cameraRender([](MeshRender* render, Camera* camera)->void {
			render->getMaterial()->getVSShader()->setUniform("g_View", camera->view());
			render->getMaterial()->getVSShader()->setUniform("g_Proj", camera->projection());
		});
		m_renderObjects.push_back(newRender);
	}
}