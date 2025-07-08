#include "IDrawer.h"

void IDrawer::init(D3D11Context* context) {
	m_context = context;
}

void IDrawer::addItem(IObject* item) {
	m_items.push_back(item);
}

void IDrawer::renderForeach() {
	for (auto item : m_items) {
		item->render();
	}
}
