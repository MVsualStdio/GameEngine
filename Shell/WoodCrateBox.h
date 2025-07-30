#pragma once

#include "../Core/MeshRender.h"


class WoodCrateBox : public MeshRender {
public:
	WoodCrateBox(IDrawer* draw, D3D11Context* context, Texture2D& texture);
	void tick(double dt) override;
	void cameraChange(const Eigen::Matrix4f& view, const Eigen::Matrix4f& projection) override;
private:
	double m_time = 0.0;
	Texture2D m_texture;
};
