#include "DrawScreen.h"


void DrawScreen::present(double dt) {
	//m_context->setOMScreenRT();
	m_context->ClearScreenRT();

	renderForeach();

	m_context->Present();
}

void DrawScreen::init(D3D11Context* context) {
	IDrawer::init(context);
}

ComPtr<ID3D11RenderTargetView> DrawScreen::getRenderTarget() {
	return m_context->getScreenRT();
}