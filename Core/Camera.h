#pragma once
#include "Eigen/Core"
#include "Eigen/Dense"
#include <DirectXMath.h>
#include "Component/Component.h"

class IDrawer;
class Camera : public Component{
public:
	Camera();
	virtual ~Camera() = default;

	void setProjection(Eigen::Vector3f target, float aspectRatio = 16.0f / 9.0f,
		float fov = 45.0f, float zNear = 1.0f, float zFar = 1000.0f);

	void setOrtho(Eigen::Vector3f target, float width, float height, float zNear = 1.0f, float zFar = 1000.0f);

	void setCullMask(unsigned char cullingMask) { m_cullingMask = cullingMask; }
	unsigned char getCullMask() { return m_cullingMask; }

	void setRenderPass(IDrawer* drawer) { m_drawer = drawer; }
	IDrawer* getRenderPass() { return m_drawer; }

	void setRender(bool needRender) { m_render = needRender; }
	bool needRender() { return m_render; }

	static std::vector<Camera*> CameraList();

	const Eigen::Matrix4f view();
	const Eigen::Matrix4f projection();

	Eigen::Vector3f front() { return m_front; }
	Eigen::Vector3f right() { return m_right; }
	Eigen::Vector3f up() { return m_up; }
	void setFront(Eigen::Vector3f front) {  
		m_front = front; 
		updateCoord();
	}

private:
	float m_fov;
	float m_aspectRatio;
	float m_zNear;
	float m_zFar;

	float m_width;
	float m_height;

	const double M_PI = 3.14159265358979323846;

	Eigen::Vector3f m_front;
	Eigen::Vector3f m_right;
	Eigen::Vector3f m_y;
	Eigen::Vector3f m_up;
	Eigen::Vector3f m_target;
	bool m_viewDirty;

	Eigen::Matrix4f m_view;
	Eigen::Matrix4f m_projection;

	unsigned char m_cullingMask;
	IDrawer* m_drawer;
	bool m_render;

	void copy(Eigen::Matrix4f& mat, DirectX::XMMATRIX& dirMat);
	void updateCoord();
	void updateViewMatrix();
	void updateProjectionMatrix();
	void updateOrtho();

	static std::vector<Camera*> gAllcamera;

};


