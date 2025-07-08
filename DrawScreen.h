#pragma once

#include "IDrawer.h"
#include <vector>

class DrawScreen : public IDrawer {
public:
	void present(double dt) override;
	void init(D3D11Context* context) override;

};

