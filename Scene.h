#pragma once

#include "DrawManger.h"
#include "MeshRender.h"



class WoodCrateBox : public MeshRender, public IGameObject {
public:
	WoodCrateBox(IDrawer* draw, D3D11Context* context, ICamera* camera);
	void tick(double dt) override;
private:
	double m_time = 0.0;
};



class DrawMangerTexture : public DrawMangerBase {
public:
	DrawMangerTexture();
private:
	virtual void initDrawer();
	virtual void initCompent();
private:
	void initDrawScreen(IDrawer* draw);
	void initDrawTexture(IDrawer* draw);
	std::shared_ptr<Texture2D> texture;
	double m_time = 0.0;
};