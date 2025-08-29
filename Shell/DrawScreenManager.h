#pragma once

#include "../Core/DrawManger.h"
#include "../Core/DrawScreen.h"
#include "../Core/RenderPass.h"


class DrawMangerScreen : public DrawMangerBase {
public:
	DrawMangerScreen();
private:
	void initCompent() override;
protected:
	void initTranglePass();
private:
	double m_time = 0.0;
	DrawScreen* m_drawScreen;
	RenderPass* m_Tranglepass;
};