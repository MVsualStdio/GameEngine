#include "DrawScreen.h"


void DrawScreen::present(double dt) {
	m_context->resetRT();
	m_context->ClearScreenRT();
	renderForeach(dt);
	m_context->Present();
}

void DrawScreen::init(D3D11Context* context) {
	IDrawer::init(context);
}

ComPtr<ID3D11RenderTargetView> DrawScreen::getRenderTarget() {
	return m_context->getScreenRT();
}

DrawTexture::DrawTexture(Texture2D* texture)
	: m_texture(texture) {

}

void DrawTexture::present(double dt) {
	m_context->resetRT();
	m_context->ClearRT(m_texture->GetRenderTarget());
	renderForeach(dt);
}

void DrawTexture::init(D3D11Context* context) {
	IDrawer::init(context);
}

ComPtr<ID3D11RenderTargetView> DrawTexture::getRenderTarget() {
	return m_texture->GetRenderTarget();
}
