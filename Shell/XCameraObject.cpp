#include "XCameraObject.h"

#include "../Core/InputControl.h"

XCameraObject::XCameraObject()
	: GameObject("XCamera") {

}
XCameraObject::~XCameraObject() {

}

void XCameraObject::init(IDrawer* drawer, D3D11Context* context) {
	m_camera = dynamic_cast<Camera*>(addComponent("Camera"));
	m_cameraTransfrom = dynamic_cast<Transform*>(getComponent("Transform"));
	if (!m_cameraTransfrom) {
		m_cameraTransfrom = dynamic_cast<Transform*>(addComponent("Transform"));
	}
	m_cameraTransfrom->setPosition(Eigen::Vector3f{ 10,10,-10 });
	m_camera->setProjection(Eigen::Vector3f{ 0,0,0 }, context->AspectRatio());
	m_camera->setRenderPass(drawer);
}

void XCameraObject::update(double dt) {
	if (InputControl::hasKey(KEY_CODE_W)) {
		Eigen::Vector3f front = m_camera->front() + Eigen::Vector3f{ 0, 0, 0.1 };
		m_cameraTransfrom->offsetPosition(front);
	}
	if (InputControl::hasKey(KEY_CODE_S)) {
		Eigen::Vector3f front = -(m_camera->front() + Eigen::Vector3f{ 0, 0, 0.1 });
		m_cameraTransfrom->offsetPosition(front);
	}
	if (InputControl::hasKey(KEY_CODE_A)) {
		Eigen::Vector3f right = -(m_camera->right() + Eigen::Vector3f{ -0.1, 0, 0 });
		m_cameraTransfrom->offsetPosition(right);
	}
	if (InputControl::hasKey(KEY_CODE_D)) {
		Eigen::Vector3f right = m_camera->right() + Eigen::Vector3f{ 0.1, 0, 0 };
		m_cameraTransfrom->offsetPosition(right);
	}
	if (InputControl::hasMouse()) {
		auto dM = InputControl::dMouseMove();
		float yawRad = dM[0] * M_PI / 180.0f;
		float pitchRad = dM[1] * M_PI / 180.0f;

		Eigen::Quaternionf qYaw(Eigen::AngleAxisf(yawRad, m_camera->up()));
		Eigen::Quaternionf qPitch(Eigen::AngleAxisf(pitchRad, m_camera->right()));
		Eigen::Quaternionf totalRot = qYaw * qPitch;

		m_camera->setFront(totalRot * m_camera->front());
	}
}