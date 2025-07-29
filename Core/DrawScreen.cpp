#include "DrawScreen.h"


void DrawScreen::present(double dt) {
	m_context->resetRT();
	m_context->ClearScreenRT();
	renderForeach(dt);
	m_context->Present();
}

 DrawScreen::DrawScreen(D3D11Context* context) 
	:IDrawer(context){

}

ComPtr<ID3D11RenderTargetView> DrawScreen::getRenderTarget() {
	return m_context->getScreenRT();
}


