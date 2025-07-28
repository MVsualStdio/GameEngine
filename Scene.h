#pragma once

#include "DrawManger.h"
#include "MeshRender.h"



class DrawMangerTexture : public DrawMangerBase {
public:
	DrawMangerTexture();
private:
	void initDrawer() override;
	void initCompent() override;
private:
	void initDrawScreen(IDrawer* draw);
	void initDrawTexture(IDrawer* draw);
	std::shared_ptr<Texture2D> texture;
	double m_time = 0.0;
};