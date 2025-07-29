#pragma once

#include "../Core/DrawManger.h"
#include "../Core/MeshRender.h"
#include "../Core/Scene.h"
#include "../Core/DrawScreen.h"
#include "../Core/RenderPass.h"

class DrawMangerScreen : public DrawMangerBase {
public:
	DrawMangerScreen();
private:
	void prepare() override;
	void initCompent() override;
protected:
	void initScene();
	void initTranglePass();
private:
	double m_time = 0.0;
	std::shared_ptr<Scene> m_scene;
	DrawScreen* m_drawScreen;
	RenderPass* m_Tranglepass;
};