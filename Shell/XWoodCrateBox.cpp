#include <rttr/registration>
#include "XWoodCrateBox.h"
#include "../Core/DDSTextureLoader11.h"
#include "../Core/MeshFilter.h"

#include "Eigen/Dense"
#include "../Core/Component/Component.h"
#include "../Core/VertexLayout.h"


XWoodCrateBox::XWoodCrateBox()
	: GameObject("XWoodCrateBox") {

}

void XWoodCrateBox::init(IDrawer* drawer, D3D11Context* context, Texture2D* texture) {
	m_texture = texture;

	m_render = dynamic_cast<MeshRender*>(addOnlyComponent("MeshRender"));
	m_transform = dynamic_cast<Transform*>(addOnlyComponent("Transform"));
	m_light = dynamic_cast<XLightCpt*>(addOnlyComponent("XLight"));

	m_render->init(drawer, context);

	ID3D11ShaderResourceView* textureView;
	DirectX::CreateDDSTextureFromFile(context->m_Device.Get(), L"D:/work/GameEngine/HLSL/WoodCrate.dds", nullptr, &textureView);
	Texture2D textureDDS(context, textureView);

	Material* material = new Material(context);
	material->setVSShader(L"D:/work/GameEngine/HLSL/woodCrateBoxLight.hlsli");
	material->setPSShader(L"D:/work/GameEngine/HLSL/woodCrateBoxLight.hlsli");
	
	Eigen::Matrix4f world = m_transform->getMatrix();

	material->getVSShader()->setUniform("g_World", world);
	Eigen::Matrix4f worldInv = world.inverse().transpose();
	material->getVSShader()->setUniform("g_WorldInvTranspose", worldInv);
	material->getPSShader()->setUniform("lightPosition", m_light->getPosition());

	Eigen::Vector3f color = Eigen::Vector3f{ m_light->getLightColor().x(),m_light->getLightColor().y(),m_light->getLightColor().z() };
	material->getPSShader()->setUniform("lightColor", color);

	material->getPSShader()->setTexture(0, textureDDS);
	material->getPSShader()->setTexture(1, *m_texture);
	m_render->setMaterial(std::shared_ptr<Material>(material));

	//m_render->setVertex(Geometry::CreateCube(2.0f, 2.0f, 2.0f));
	std::string path = "C:/Users/41795/Downloads/jiggly_watermelon_jello/scene.gltf";
	mesh = MeshFilter::instance()->getMeshInfo(path);
	
	for (int i = 0; i < mesh.size(); ++i) {
		std::shared_ptr<VertexBuffer<VertexUV>> vertex = mesh[i].vertexs->vertex<VertexUV>();
		m_render->setVertex(mesh[i].vertexs);
	}
	Model model = MeshFilter::instance()->getModel(path);
	anim = new KeyAnimation(path, &model);

	m_render->cameraRender([](MeshRender* render, Camera* camera)->void {
		render->getMaterial()->getVSShader()->setUniform("g_View", camera->view());
		render->getMaterial()->getVSShader()->setUniform("g_Proj", camera->projection());

		Transform* trans = dynamic_cast<Transform*>(camera->gameObject()->getComponent("Transform"));
		render->getMaterial()->getPSShader()->setUniform("eyePosW", trans->getPosition());
	});
}

void XWoodCrateBox::update(double dt) {
	m_time += dt * 0.001;
	anim->update(m_time);
	std::unordered_map<int, Eigen::Matrix4f> matrixs = anim->getfinalVertex();
	for (int i = 0; i < mesh.size(); ++i) {
		std::shared_ptr<VertexBuffer<VertexUV>> vertex = mesh[i].vertexs->vertex<VertexUV>();
		Eigen::Vector3<float>& pos = vertex->vertices[vertex->bonesWidget[i].vertexIndex].pos;
		Eigen::Vector4<float> pos4 = { pos.x(),pos.y() ,pos.z() ,1.0f };
		pos4 = matrixs[vertex->bonesWidget[i].boneIndex] * pos4;
		pos4 = pos4 / pos4.w();
		pos = { pos4.x(),pos4.y(),pos4.z() };
	}

	//float RotationSpeed = 1.0f / 1000.0f;
	//m_time += RotationSpeed * dt;
	//Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
	//world.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_time, Eigen::Vector3f::UnitX()).toRotationMatrix();
	//Eigen::Matrix4f norworld = world.inverse().transpose();
	//m_render->getMaterial()->getVSShader()->setUniform("g_WorldInvTranspose", norworld);
	//m_render->getMaterial()->getVSShader()->setUniform("g_World", world);
}