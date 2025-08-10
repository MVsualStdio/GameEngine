#pragma once

#include "IDrawer.h"
#include <vector>
#include "Texture.h"

class DrawScreen : public IDrawer {
public:
	DrawScreen(D3D11Context* context);
	void present() override;
	void clear() override;

	ComPtr<ID3D11RenderTargetView> getRenderTarget() override;
};
