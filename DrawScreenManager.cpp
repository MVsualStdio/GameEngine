#include "DrawScreenManager.h"
#include "DrawScreen.h"

#include "DDSTextureLoader11.h"
#include <Eigen/Geometry>
#include "GameObjBox.h"


DrawMangerScreen::DrawMangerScreen() {

}

void DrawMangerScreen::initCompent() {
	std::shared_ptr<ICamera> camera = std::make_shared<ProjectionCamera>(Eigen::Vector3f{ 0,0,-30 }, Eigen::Vector3f{ 0,0,0 }, m_context->AspectRatio());
	addMainCamera(0, camera);
}

void DrawMangerScreen::initDrawer() {
	DrawScreen* drawScreen = new DrawScreen();
	drawScreen->init(m_context.get());
	drawScreen->initDrawFunction([this](IDrawer* draw) {
		std::shared_ptr<MeshRender> box = std::make_shared<WoodCrateBox>(draw, m_context.get(), getMainCamera());
		draw->addItem(box);
	});

	m_drawList.push_back(drawScreen);
}