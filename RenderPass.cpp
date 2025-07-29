#include "RenderPass.h"

//RenderPass::RenderPass(Texture2D* texture)
//	: m_texture(texture) {
//
//}

RenderPass::RenderPass(D3D11Context* context) 
	: IDrawer(context)
	, m_texture(std::make_shared<Texture2D>(m_context, m_context->width(), m_context->height())) {

}

void RenderPass::present(double dt) {
	m_context->resetRT();
	m_context->ClearRT(m_texture->GetRenderTarget());
	renderForeach(dt);
}

ComPtr<ID3D11RenderTargetView> RenderPass::getRenderTarget() {
	return m_texture->GetRenderTarget();
}

Texture2D* RenderPass::getResult() {
	return m_texture.get();
}