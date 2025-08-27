#include <rttr/registration>
#include "XCarAnimation.h"
#include "../Core/DDSTextureLoader11.h"
#include "../Core/MeshFilter.h"

#include "Eigen/Dense"
#include "../Core/Component/Component.h"


XCarAnimation::XCarAnimation()
	: GameObject("XCarAnimation") {

}

void XCarAnimation::init(IDrawer* drawer, D3D11Context* context, Texture2D* texture) {
	m_texture = texture;

	std::string path = "C:/Users/41795/Downloads/low-poly_cartoon_style_car_03/scene.gltf";
	LoadMesh load(path);
	m_nodeMesh = load.getNodeMesh();
	m_anim = new Animation(path);
	m_anim->update(0);
	auto AnimMat = m_anim->getAnimMat();

	int meshSize = m_nodeMesh.size();

	for (auto node : m_nodeMesh) {

		MeshRender* render = dynamic_cast<MeshRender*>(addComponent("MeshRender"));
		m_transform = dynamic_cast<Transform*>(addOnlyComponent("Transform"));
		m_light = dynamic_cast<XLightCpt*>(addOnlyComponent("XLight"));

		render->init(drawer, context);

		ID3D11ShaderResourceView* textureView;
		DirectX::CreateDDSTextureFromFile(context->m_Device.Get(), L"D:/work/GameEngine/HLSL/WoodCrate.dds", nullptr, &textureView);
		Texture2D textureDDS(context, textureView);

		Material* material = new Material(context);
		material->setVSShader(L"D:/work/GameEngine/HLSL/woodCrateBoxLight.hlsli");
		material->setPSShader(L"D:/work/GameEngine/HLSL/woodCrateBoxLight.hlsli");

		Eigen::Matrix4f world = AnimMat[node.first];
		//Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
		material->getVSShader()->setUniform("g_World", world);
		Eigen::Matrix4f worldInv = world.inverse().transpose();
		material->getVSShader()->setUniform("g_WorldInvTranspose", worldInv);
		material->getPSShader()->setUniform("lightPosition", m_light->getPosition());

		Eigen::Vector3f color = Eigen::Vector3f{ m_light->getLightColor().x(),m_light->getLightColor().y(),m_light->getLightColor().z() };
		material->getPSShader()->setUniform("lightColor", color);

		material->getPSShader()->setTexture(0, textureDDS);
		material->getPSShader()->setTexture(1, *m_texture);
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
	m_anim->update(dt);
	auto AnimMat = m_anim->getAnimMat();
	for (auto render : m_render) {
		Eigen::Matrix4f world = AnimMat[render.first];
		render.second->getMaterial()->getVSShader()->setUniform("g_World", world);
	}

}