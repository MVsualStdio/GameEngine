#pragma once

#include "IDrawer.h"
#include "D3D11Context.h"
#include <memory>
#include <set>

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
	struct DrawerPtrComparator {
		bool operator()(IDrawer* a, IDrawer* b) const {
			if (a && b) {
				if (a->getRenderOrder() != b->getRenderOrder()) {
					return a->getRenderOrder() < b->getRenderOrder();
				}
				return a < b;
			}
			return a < b;
		}
	};

	std::set<IDrawer*, DrawerPtrComparator> m_drawList;
	std::shared_ptr<D3D11Context> m_context;
	
};


