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

	ComPtr<ID3D11RenderTargetView> getRenderTarget() override;
	Texture2D* getResult();
private:
	std::shared_ptr<Texture2D> m_texture;
};
