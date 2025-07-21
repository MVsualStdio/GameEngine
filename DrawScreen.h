#pragma once

#include "IDrawer.h"
#include <vector>
#include "Texture.h"

class DrawScreen : public IDrawer {
public:
	void present(double dt) override;
	void init(D3D11Context* context) override;
	ComPtr<ID3D11RenderTargetView> getRenderTarget() override;
};

class DrawTexture : public IDrawer {
public:
	DrawTexture(Texture2D* texture);
	void present(double dt) override;
	void init(D3D11Context* context) override;
	ComPtr<ID3D11RenderTargetView> getRenderTarget() override;
private:
	Texture2D* m_texture;
};
