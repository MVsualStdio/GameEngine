#include "DrawScreen.h"

 DrawScreen::DrawScreen(D3D11Context* context) 
	:IDrawer(context){

}

ComPtr<ID3D11RenderTargetView> DrawScreen::getRenderTarget() {
	return m_context->getScreenRT();
}

void DrawScreen::present() {
	m_context->Present();
}

void DrawScreen::clear() {
	m_context->resetRT();
	m_context->ClearScreenRT();
}
