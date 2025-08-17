#include "DrawScreen.h"

 DrawScreen::DrawScreen(D3D11Context* context) 
	:IDrawer(context){

}

Texture2D* DrawScreen::getRenderTarget() {
	return m_context->getScreenRT();
}

Depth2D* DrawScreen::getDepthStencil() {
	return m_context->getScreenDepth();
}

void DrawScreen::present() {
	m_context->Present();
}

void DrawScreen::clear() {
	m_context->resetRT();
	m_context->ClearScreenRT();
	m_context->ClearScreenDepth();
}
