#pragma once

#include "DrawManger.h"
#include "MeshRender.h"
#include "Scene.h"
#include "DrawScreen.h"

class DrawMangerScreen : public DrawMangerBase {
public:
	DrawMangerScreen();
private:
	void prepare() override;
	void initCompent() override;
private:
	double m_time = 0.0;
	std::shared_ptr<Scene> m_scene;
	DrawScreen* m_drawScreen;
};