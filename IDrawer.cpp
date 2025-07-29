#include "IDrawer.h"

IDrawer::IDrawer(D3D11Context* context)
	: m_context(context) {
}

void IDrawer::addItem(std::shared_ptr<IRenderObject> item) {
	m_items.push_back(std::move(item));
}

void IDrawer::renderForeach(double dt) {
	for (auto item = m_items.begin(); item != m_items.end(); ++item) {
		(*item)->render(dt);
	}
}

void IDrawer::updateCamera(ICamera* camera) {
	for (auto item = m_items.begin(); item != m_items.end(); ++item) {
		(*item)->updateCamera(camera);
	}
}

std::vector<std::shared_ptr<IRenderObject>> IDrawer::getAllItems() {
	return m_items;
}