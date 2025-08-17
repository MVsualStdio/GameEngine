#pragma once

#include "IDrawer.h"
#include <vector>
#include "Texture.h"
#include "D3D11Context.h"

class RenderPass : public IDrawer {
public:
	//RenderPass(Texture2D* texture);
	RenderPass(D3D11Context* context);

	void present() override;
	void clear() override;

	Texture2D* getRenderTarget() override;
	Depth2D* getDepthStencil() override;
private:
	std::shared_ptr<Texture2D> m_texture;
	std::shared_ptr<Depth2D> m_depth;
};
