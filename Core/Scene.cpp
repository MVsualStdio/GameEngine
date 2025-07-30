#include "Scene.h"
#include "DrawScreen.h"

#include "DDSTextureLoader11.h"
#include "MeshFilter.h"
#include <Eigen/Geometry>

Scene::Scene(D3D11Context* context, IDrawer* drawer)
	: m_context(context)
	, m_drawer(drawer) {

}

Scene::~Scene() {

}

void Scene::render(double dt) {
	for (const auto& [name, mesh] : m_renderItems) {
		mesh->render(dt);
	}
}

void Scene::addMeshRender(const std::string& name, std::shared_ptr<MeshRender> mesh) {
	m_renderItems[name] = mesh;
}

std::shared_ptr<MeshRender> Scene::getRender(const std::string& name) {
	if (m_renderItems.find(name) != m_renderItems.end()) {
		return m_renderItems[name];
	}
	return nullptr;
}

const Scene::RenderItem& Scene::getAllRenderItems() {
	return m_renderItems;
}

void Scene::setDrawer(IDrawer* drawer) {
	m_drawer = drawer;
}

void Scene::setProjection(Eigen::Matrix4f& projection) {
	for (const auto& [name, mesh] : m_renderItems) {
		mesh->setProjection(projection);
	}
}

void Scene::setView(Eigen::Matrix4f& view) {
	for (const auto& [name, mesh] : m_renderItems) {
		mesh->setView(view);
	}
}