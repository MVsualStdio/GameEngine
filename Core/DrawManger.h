#pragma once

#include "IDrawer.h"
#include "D3D11Context.h"
#include <memory>
#include <unordered_set>

class DrawMangerBase {
public:
	DrawMangerBase();
	virtual ~DrawMangerBase();
	void present(double dt);
	void init(HWND winID, uint32_t width, uint32_t height);
	int getDrawSize();
	//IDrawer* getDrawer(int index);
	IDrawer* getScreenDrawer();

protected:
	virtual void initCompent() {};

protected:
	std::unordered_set<IDrawer*> m_drawList;
	std::shared_ptr<D3D11Context> m_context;
	
};


