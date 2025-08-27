#include "DrawManger.h"
#include "DrawScreen.h"
#include "MeshRender.h"
#include "DDSTextureLoader11.h"
#include "MeshFilter.h"
#include "Component/GameObject.h"
#include "InputControl.h"
DrawMangerBase::DrawMangerBase(){

}

DrawMangerBase::~DrawMangerBase() {
	for (IDrawer* drawer : m_drawList) {
		delete drawer;
	}
}

void DrawMangerBase::present(double dt) {
	
	for (auto component : Component::getAllComponents()) {
		component->update(dt);
	}
	for (auto gameObject : GameObject::getALLGameObject()) {
		gameObject->update(dt);
	}
	for (IDrawer* drawer : m_drawList) {
		drawer->clear();
		for (auto camera : Camera::CameraList()) {
			if (camera->getRenderPass() == drawer && camera->needRender()) {
				drawer->onDraw(camera);
			}
		}
		drawer->present();
	}
	InputControl::clear();

}

void DrawMangerBase::init(HWND winID, uint32_t width, uint32_t height) {
	m_context = std::make_shared<D3D11Context>(winID, width, height);
	initCompent();
	
	for (auto gameObj : GameObject::getALLGameObject()) {
		std::vector<Component*> components = gameObj->getComponents("MeshRender");
		for (Component* component : components) {
			if (!component) {
				continue;
			}
			MeshRender* render = dynamic_cast<MeshRender*>(component);
			if (!render) {
				continue;
			}
			if (m_drawList.find(render->getPass()) == m_drawList.end()) {
				m_drawList.emplace(render->getPass());
			}
			auto drawer = m_drawList.find(render->getPass());
			(*drawer)->addItem(render);
		}

	}
}

int DrawMangerBase::getDrawSize() {
	return m_drawList.size();
}

//IDrawer* DrawMangerBase::getDrawer(int index) {
//	return m_drawList[index];
//}

IDrawer* DrawMangerBase::getScreenDrawer() {
	for (auto& drawer : m_drawList) {
		DrawScreen* drawScreen = dynamic_cast<DrawScreen*>(drawer);
		if (drawScreen) {
			return drawScreen;
		}
	}
	return nullptr;
}