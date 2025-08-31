#include "IDrawer.h"
#include "MeshRender.h"

IDrawer::IDrawer(D3D11Context* context)
	: m_context(context) {
}

void IDrawer::addItem(MeshRender* item) {
	m_items.push_back(item);
}

void IDrawer::onDraw(Camera* camera) {
	for (auto item = m_items.begin(); item != m_items.end(); ++item) {
		(*item)->render(camera);
	}
}

std::vector<MeshRender*> IDrawer::getAllItems() {
	return m_items;
}

int IDrawer::getRenderOrder() const {
	return m_order;
}

void IDrawer::setRenderOrder(int order) {
	m_order = order;
}