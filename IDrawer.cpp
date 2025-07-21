#include "IDrawer.h"

void IDrawer::init(D3D11Context* context) {
	m_context = context;
}

void IDrawer::addItem(std::unique_ptr<IRenderObject> item) {
	m_items.push_back(std::move(item));
}

void IDrawer::renderForeach() {
	for (auto item = m_items.begin(); item != m_items.end(); ++item) {
		(*item)->render();
	}
}

void IDrawer::initDrawFunction(DrawFunction op) {
	m_op = op;
}

void IDrawer::initRender() {
	m_op(this);
}
