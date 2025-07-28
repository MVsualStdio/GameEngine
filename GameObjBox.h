#pragma once

#include "MeshRender.h"


class WoodCrateBox : public MeshRender {
public:
	WoodCrateBox(IDrawer* draw, D3D11Context* context, ICamera* camera);
	void tick(double dt) override;
	void cameraChange() override;
private:
	double m_time = 0.0;
};
