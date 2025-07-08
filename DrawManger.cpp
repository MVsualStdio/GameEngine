#include "DrawManger.h"
#include "DrawScreen.h"


DrawMangerBase::DrawMangerBase() {

}

DrawMangerBase::~DrawMangerBase() {
	
}

void DrawMangerBase::present(double dt) {
	for (IDrawer* drawer : m_drawList) {
		drawer->present(dt);
	}
}

void DrawMangerBase::init(HWND winID, uint32_t width, uint32_t height) {
	DrawScreen* drawScreen = new DrawScreen();
	m_context = std::make_shared<D3D11Context>(winID, width, height);
	drawScreen->init(m_context.get());
	m_drawList.push_back(drawScreen);
}
