#pragma once

#include <vector>
#include "IDrawer.h"
#include "D3D11Context.h"
#include <memory>

class DrawMangerBase {
public:
	DrawMangerBase();
	virtual ~DrawMangerBase();
	void present(double dt);
	void init(HWND winID, uint32_t width, uint32_t height);
	void initMeshRender();
protected:
	virtual void initDrawer() = 0;
protected:
	std::vector<IDrawer*> m_drawList;
	std::shared_ptr<D3D11Context> m_context;
};


class DrawMangerTexture : public DrawMangerBase {
private:
	virtual void initDrawer();
private:
	void initDrawScreen(IDrawer* draw);
	void initDrawTexture(IDrawer* draw);
	Texture2D* texture;
};