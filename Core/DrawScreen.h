#pragma once

#include "IDrawer.h"
#include <vector>
#include "Texture.h"

class DrawScreen : public IDrawer {
public:
	DrawScreen(D3D11Context* context);
	void present() override;
	void clear() override;

	Texture2D* getRenderTarget() override;
	Depth2D* getDepthStencil() override;
};
