#pragma once

#include "../Core/MeshRender.h"
#include "../Core/Scene.h"

class WoodCrateBox : public MeshRender {
public:
	WoodCrateBox(IDrawer* draw, D3D11Context* context, Scene* scene, Texture2D& texture);
	void tick(double dt) override;
	void cameraChange(ICamera* canera) override;
private:
	double m_time = 0.0;
	Texture2D m_texture;
	Scene* m_scene;
};
