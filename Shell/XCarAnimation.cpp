#include <rttr/registration>
#include "XCarAnimation.h"
#include "../Core/DDSTextureLoader11.h"
#include "../Core/MeshFilter.h"
#include "../Core/Component/Component.h"
#include "../Core/TextureManager.h"
#include "../Core/FileSystem.h"

XCarAnimation::XCarAnimation()
	: GameObject("XCarAnimation") {

}

void XCarAnimation::init(IDrawer* drawer, D3D11Context* context) {

	std::string path = FileSystem::HLSLPath("/car/scene.gltf");

	LoadMesh load(path);
	m_nodeMesh = load.getNodeMesh();
	m_anim = dynamic_cast<Animation*>(addOnlyComponent("Animation"));;
	m_anim->init(path);

	m_transform = dynamic_cast<Transform*>(addOnlyComponent("Transform"));
	m_light = dynamic_cast<XLightCpt*>(addOnlyComponent("XLight"));

	auto AnimMat = m_anim->getAnimMat();

	for (auto node : m_nodeMesh) {

		NodeMesh mesh = node.second;
		
		MeshRender* render = dynamic_cast<MeshRender*>(addComponent("MeshRender"));


		render->init(drawer, context);

		Material* material = new Material(context);
		material->setVSShader(FileSystem::HLSLWPath("/BoneAnimation.hlsli").data());
		material->setPSShader(FileSystem::HLSLWPath("/BoneAnimation.hlsli").data());
		
		Eigen::Matrix4f world = AnimMat[node.first];

		material->getVSShader()->setUniform("g_World", world);
		Eigen::Matrix4f worldInv = world.inverse().transpose();
		material->getVSShader()->setUniform("g_WorldInvTranspose", worldInv);
		material->getPSShader()->setUniform("lightPosition", m_light->getPosition());

		Eigen::Vector3f color = Eigen::Vector3f{ m_light->getLightColor().x(),m_light->getLightColor().y(),m_light->getLightColor().z() };
		material->getPSShader()->setUniform("lightColor", color);

		for (int i = 0; i < mesh.textures.size(); ++i) {
			Texture2D* texture = TextureManager::instance()->getTexture(mesh.textures[i].path, context);
			material->getPSShader()->setTexture(i, texture);
		}
		
		render->setMaterial(std::shared_ptr<Material>(material));

		render->setVertex(std::make_shared<AnyVertexBuffer>(node.second.vertexs));

		m_render[node.first] = render;

		render->cameraRender([](MeshRender* render, Camera* camera)->void {
			render->getMaterial()->getVSShader()->setUniform("g_View", camera->view());
			render->getMaterial()->getVSShader()->setUniform("g_Proj", camera->projection());

			Transform* trans = dynamic_cast<Transform*>(camera->gameObject()->getComponent("Transform"));
			render->getMaterial()->getPSShader()->setUniform("eyePosW", trans->getPosition());
		});
	}

}

void XCarAnimation::update(double dt) {
	auto AnimMat = m_anim->getAnimMat();
	for (auto render : m_render) {
		Eigen::Matrix4f world = AnimMat[render.first];
		render.second->getMaterial()->getVSShader()->setUniform("g_World", world);
	}
}