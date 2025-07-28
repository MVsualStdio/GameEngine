#pragma once

#include "DrawManger.h"
#include "MeshRender.h"

class DrawMangerScreen : public DrawMangerBase {
public:
	DrawMangerScreen();
private:
	void initDrawer() override;
	void initCompent() override;
private:
	double m_time = 0.0;
};