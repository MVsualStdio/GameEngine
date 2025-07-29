#include "DrawScreenManager.h"


#include "DDSTextureLoader11.h"
#include <Eigen/Geometry>
#include "GameObjBox.h"


DrawMangerScreen::DrawMangerScreen() {

}

void DrawMangerScreen::initCompent() {
	std::shared_ptr<ICamera> camera = std::make_shared<ProjectionCamera>(Eigen::Vector3f{ 0,0,-30 }, Eigen::Vector3f{ 0,0,0 }, m_context->AspectRatio());
	addMainCamera(0, camera);
	std::shared_ptr<ICamera> camera2 = std::make_shared<ProjectionCamera>(Eigen::Vector3f{ 30,30,-30 }, Eigen::Vector3f{ 0,0,0 }, m_context->AspectRatio());
	addOtherCamera(1, camera2);

	m_drawScreen = new DrawScreen(m_context.get());
	m_drawList.push_back(m_drawScreen);

	m_scene = std::make_shared<Scene>(m_context.get(), m_drawScreen);
	m_drawScreen->addItem(m_scene);
	
	std::shared_ptr<MeshRender> box = std::make_shared<WoodCrateBox>(m_drawScreen, m_context.get(), getMainCamera());
	m_scene->addMeshRender("box", box);
	
}

void DrawMangerScreen::prepare() {
	
}