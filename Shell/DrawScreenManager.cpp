#include "DrawScreenManager.h"
#include "../Core/DDSTextureLoader11.h"
#include <Eigen/Geometry>
#include "WoodCrateBox.h"
#include "../Core/MeshFilter.h"
#include "../Core/Component/GameObject.h"
#include "../Core/Component/Transform.h"

DrawMangerScreen::DrawMangerScreen() {

}

void DrawMangerScreen::initScene() {
	//std::shared_ptr<MeshRender> box = std::make_shared<WoodCrateBox>(m_drawScreen, m_context.get(), m_scene.get(), *m_Tranglepass->getResult());
	//m_scene->addMeshRender("box", box);
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

	GameObject* boxObj = new GameObject("boxObj");
	WoodCrateBox* box =  dynamic_cast<WoodCrateBox*>(boxObj->addComponent("WoodCrateBox"));
	box->init(m_drawScreen, m_context.get(), m_Tranglepass->getResult());
}
