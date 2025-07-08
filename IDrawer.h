#pragma once

#include "D3D11Context.h"
#include <vector>

class IObject {
public:
	virtual void render() = 0;
};

class IDrawer {
public:
	virtual void present(double dt) = 0;
	virtual void init(D3D11Context* context);
	void addItem(IObject* item);
	void renderForeach();
protected:
	std::vector<IObject*> m_items;
	D3D11Context* m_context = nullptr;
};

