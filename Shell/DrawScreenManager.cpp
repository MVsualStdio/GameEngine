#include "DrawScreenManager.h"
#include "../Core/DDSTextureLoader11.h"
#include <Eigen/Geometry>
#include "../Core/MeshFilter.h"
#include "../Core/Component/GameObject.h"
#include "XCameraObject.h"
#include "XWoodCrateBox.h"
#include "XPlane.h"
#include "XLight.h"
#include "XCarAnimation.h"
#include "../Core/TextureManager.h"
#include "../Core/FileSystem.h"
#include "XBoneAnimation.h"
#include "ShadowMap.h"

constexpr float XM_PIDIV2 = 1.570796327f;


DrawMangerScreen::DrawMangerScreen() {
	TextureManager::instance()->addTexture(FileSystem::HLSLPath("/floor.dds"));
	TextureManager::instance()->addTexture(FileSystem::HLSLPath("/brick.dds"));
}


void DrawMangerScreen::initTranglePass() {

	GameObject* go = new GameObject("trangle");
	Camera* camera = dynamic_cast<Camera*>(go->addComponent("Camera"));
	camera->setRenderPass(m_Tranglepass);

	MeshRender* trangle = dynamic_cast<MeshRender*>(go->addComponent("MeshRender"));
	trangle->init(m_Tranglepass, m_context.get());
	
	Material* material = new Material(m_context.get());
	material->setVSShader(L"D:/work/GameEngine/HLSL/baseVS.hlsli");
	material->setPSShader(L"D:/work/GameEngine/HLSL/basePS.hlsli");
	Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
	material->getVSShader()->setUniform("g_World", world);
	material->getVSShader()->setUniform("g_View", world);
	material->getVSShader()->setUniform("g_Proj", world);
	ID3D11ShaderResourceView* textureView;
	DirectX::CreateDDSTextureFromFile(m_context->m_Device.Get(), L"D:/work/GameEngine/HLSL/flare.dds", nullptr, &textureView);
	Texture2D textureDDS(m_context.get(), textureView);
	material->getPSShader()->setTexture(0, textureDDS);

	trangle->setMaterial(std::shared_ptr<Material>(material));
	Eigen::Vector3f points[3] = { { 0.0f, 0.5f, 0.5f },{ 0.5f, -0.5f, 0.5f },{ -0.5f, -0.5f, 0.5f } };
	trangle->setVertex(Geometry::CreateTrangle(points));

	//trangle->cameraRender([](MeshRender* render, Camera* camera) {
	//	Eigen::Matrix4f identity = Eigen::Matrix4f::Identity();
	//	render->getMaterial()->getVSShader()->setUniform("g_View", identity);
	//	render->getMaterial()->getVSShader()->setUniform("g_Proj", identity);
	//});
}

void DrawMangerScreen::initCompent() {
	//Eigen::Vector3f{ 0,0,-30 }
	m_Tranglepass = new RenderPass(m_context.get());
	initTranglePass();

	m_drawScreen = new DrawScreen(m_context.get());

	XPlane* plane = new XPlane();
	Transform* transform = dynamic_cast<Transform*>(plane->addOnlyComponent("Transform"));
	transform->setPosition({ 0.0,-1.0f,0.0f });

	Texture2D* floor = TextureManager::instance()->getTexture(FileSystem::HLSLPath("/floor.dds"), m_context.get());
	plane->init(m_drawScreen, m_context.get(), floor, Geometry::CreatePlane(20.0f, 20.0f, 5.0f, 5.0f));

	Texture2D* brick = TextureManager::instance()->getTexture(FileSystem::HLSLPath("/brick.dds"), m_context.get());

	for (int i = 0; i < 4; ++i) {
		XPlane* walls = new XPlane();
		Transform* transform = dynamic_cast<Transform*>(walls->addOnlyComponent("Transform"));
		transform->setRotate({ -XM_PIDIV2, XM_PIDIV2 * i, 0.0f});
		transform->setPosition({ i % 2 ? -10.0f * (i - 2) : 0.0f, 3.0f, i % 2 == 0 ? -10.0f * (i - 1) : 0.0f });
		walls->init(m_drawScreen, m_context.get(), brick, Geometry::CreatePlane(20.0f, 8.0f, 5.0f, 1.5f));
	}

	XWoodCrateBox* box = new XWoodCrateBox(m_drawScreen, m_context.get(), m_Tranglepass->getRenderTarget());
	
	ShadowMap* shadow = new ShadowMap(m_context.get());
	shadow->addShadowMap<VertexUV>(box);
	shadow->addShadowMap<VertexUV>(plane);

	plane->setShaderTexture(shadow->getCamera(), shadow->getResult());
	//XCarAnimation* car = new XCarAnimation();
	//car->init(m_drawScreen, m_context.get());

	//XBoneAnimation* bone = new XBoneAnimation();
	//bone->init(m_drawScreen, m_context.get());

	XLight* light = new XLight();
	light->init(m_drawScreen, m_context.get());

	XCameraObject* camera = new XCameraObject();
	camera->init(m_drawScreen, m_context.get());

	


}
