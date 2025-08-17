#include "RenderPass.h"

//RenderPass::RenderPass(Texture2D* texture)
//	: m_texture(texture) {
//
//}

RenderPass::RenderPass(D3D11Context* context) 
	: IDrawer(context)
	, m_texture(std::make_shared<Texture2D>(m_context, m_context->width(), m_context->height())) 
	, m_depth(std::make_shared<Depth2D>(m_context, m_context->width(), m_context->height())){

}

Texture2D* RenderPass::getRenderTarget() {
	return m_texture.get();
}

Depth2D* RenderPass::getDepthStencil() {
	return m_depth.get();
}

void RenderPass::present() {
	
}

void RenderPass::clear() {
	m_context->resetRT();
	m_context->ClearRT(m_texture->GetRenderTarget().Get());
	m_context->ClearDepth(m_depth->GetDepthStencil().Get());
}
