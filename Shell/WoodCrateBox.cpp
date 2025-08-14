#include <rttr/registration>
#include "WoodCrateBox.h"
#include "../Core/DDSTextureLoader11.h"
#include "../Core/MeshFilter.h"
#include "../Core/Component/GameObject.h"
#include "../Core/InputControl.h"
#include "Eigen/Dense"

RTTR_REGISTRATION
{
	rttr::registration::class_<WoodCrateBox>("WoodCrateBox")
			.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

WoodCrateBox::WoodCrateBox() {

}

void WoodCrateBox::init(IDrawer* drawer, D3D11Context* context, Texture2D* texture) {
	m_texture = texture;

	GameObject* go = new GameObject("camera");
	m_camera = dynamic_cast<Camera*>(go->addComponent("Camera"));
	m_cameraTransfrom = dynamic_cast<Transform*>(go->addComponent("Transform"));
	m_cameraTransfrom->setPosition(Eigen::Vector3f{ 0,0,-30 });
	m_camera->setProjection(Eigen::Vector3f{ 0,0,0 }, context->AspectRatio());
	m_camera->setRenderPass(drawer);

	GameObject* box = new GameObject("box");
	Component* component = box->addComponent("MeshRender");
	m_render = dynamic_cast<MeshRender*>(component);
	m_render->init(drawer, context);

	ID3D11ShaderResourceView* textureView;
	DirectX::CreateDDSTextureFromFile(context->m_Device.Get(), L"D:/work/GameEngine/HLSL/WoodCrate.dds", nullptr, &textureView);
	Texture2D textureDDS(context, textureView);

	m_transform = dynamic_cast<Transform*>(box->addComponent("Transform"));

	Material* material = new Material(context);
	material->setVSShader(L"D:/work/GameEngine/HLSL/woodCrateBoxLight.hlsli");
	material->setPSShader(L"D:/work/GameEngine/HLSL/woodCrateBoxLight.hlsli");
	
	Eigen::Matrix4f world = Eigen::Matrix4f::Identity();

	material->getVSShader()->setUniform("g_World", world);

	material->getVSShader()->setUniform("g_WorldInvTranspose", world);
	material->getPSShader()->setUniform("lightPosition", Eigen::Vector3f{ 30.0f,30.0f,-40.0f });
	material->getPSShader()->setUniform("lightColor", Eigen::Vector3f{ 1.0f,1.0f,1.0f });

	material->getPSShader()->setTexture(0, textureDDS);
	material->getPSShader()->setTexture(1, *m_texture);
	m_render->setMaterial(std::shared_ptr<Material>(material));
	m_render->setVertex(Geometry::CreateCube(10.0f, 10.0f, 10.0f));

	m_render->cameraRender([](MeshRender* render, Camera* camera)->void {
		render->getMaterial()->getVSShader()->setUniform("g_View", camera->view());
		render->getMaterial()->getVSShader()->setUniform("g_Proj", camera->projection());

		Transform* trans = dynamic_cast<Transform*>(camera->gameObject()->getComponent("Transform"));
		render->getMaterial()->getPSShader()->setUniform("eyePosW", trans->getPosition());
	});
}


void WoodCrateBox::update(double dt) {
	float RotationSpeed = 1.0f / 1000.0f;
	m_time += RotationSpeed * dt;
	Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
	world.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_time, Eigen::Vector3f::UnitX()).toRotationMatrix();
	Eigen::Matrix4f norworld = world.inverse().transpose();
	m_render->getMaterial()->getVSShader()->setUniform("g_WorldInvTranspose", norworld);
	m_render->getMaterial()->getVSShader()->setUniform("g_World", world);

	if (InputControl::hasKey(KEY_CODE_W)) {
		Eigen::Vector3f front = m_camera->front() + Eigen::Vector3f{ 0, 0, 0.1 };
		m_cameraTransfrom->offsetPosition(front);
	}
	if (InputControl::hasKey(KEY_CODE_S)) {
		Eigen::Vector3f front = -(m_camera->front() + Eigen::Vector3f{ 0, 0, 0.1 });
		m_cameraTransfrom->offsetPosition(front);
	}
	if (InputControl::hasKey(KEY_CODE_A)) {
		Eigen::Vector3f right = -(m_camera->right() + Eigen::Vector3f{ -0.1, 0, 0 });
		m_cameraTransfrom->offsetPosition(right);
	}
	if (InputControl::hasKey(KEY_CODE_D)) {
		Eigen::Vector3f right = m_camera->right() + Eigen::Vector3f{ 0.1, 0, 0 };
		m_cameraTransfrom->offsetPosition(right);
	}
	if (InputControl::hasMouse()) {
		auto dM = InputControl::dMouseMove();
		float yawRad = dM[0] * M_PI / 180.0f;
		float pitchRad = dM[1] * M_PI / 180.0f;

		Eigen::Quaternionf qYaw(Eigen::AngleAxisf(yawRad, m_camera->up()));
		Eigen::Quaternionf qPitch(Eigen::AngleAxisf(pitchRad, m_camera->right()));
		Eigen::Quaternionf totalRot = qYaw * qPitch;

		m_camera->setFront(totalRot * m_camera->front());
	}
}