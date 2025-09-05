#include <rttr/registration>
#include "XBoneAnimation.h"
#include "../Core/DDSTextureLoader11.h"
#include "../Core/MeshFilter.h"
#include "../Core/Component/Component.h"
#include "../Core/TextureManager.h"
#include "../Core/FileSystem.h"

XBoneAnimation::XBoneAnimation()
	: GameObject("XBoneAnimation") {

}

void XBoneAnimation::init(IDrawer* drawer, D3D11Context* context) {

	std::string path = FileSystem::HLSLPath("/cortina/scene.gltf");
	//std::string path = FileSystem::HLSLPath("/curtainset/scene.gltf");
	//std::string path = FileSystem::HLSLPath("/vampire/dancing_vampire.dae");
	//std::string path = FileSystem::HLSLPath("/car/scene.gltf");
	LoadMesh load(path);
	m_nodeMesh = load.getNodeMesh();
	m_anim = dynamic_cast<Animation*>(addOnlyComponent("Animation"));;
	m_success = m_anim->init(path);
	auto AnimMat = m_anim->getAnimMat();
	
	for (auto node : m_nodeMesh) {

		NodeMesh mesh = node.second;

		MeshRender* render = dynamic_cast<MeshRender*>(addComponent("MeshRender"));
		m_transform = dynamic_cast<Transform*>(addOnlyComponent("Transform"));
		m_light = dynamic_cast<XLightCpt*>(addOnlyComponent("XLight"));

		render->init(drawer, context);

		Material* material = new Material(context);
		material->setVSShader(FileSystem::HLSLWPath("/BoneAnimation.hlsli").data());
		material->setPSShader(FileSystem::HLSLWPath("/BoneAnimation.hlsli").data());

		Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
		if (m_success) {
			Eigen::Matrix4f world = AnimMat[node.first];
		}
		
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

		if (mesh.textures.size() == 0) {
			std::string texturePath = "D:/work/GameEngine/HLSL/WoodCrate.dds";
			TextureManager::instance()->addTexture(texturePath);
			Texture2D* texture = TextureManager::instance()->getTexture(texturePath, context);
			material->getPSShader()->setTexture(0, texture);
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

void XBoneAnimation::update(double dt) {

	if (m_success) {
		m_anim->updateHelper(m_render, m_nodeMesh);
	}

}