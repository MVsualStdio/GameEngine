#pragma once
#include "Eigen/Core"
#include "Eigen/Dense"
#include <DirectXMath.h>

class ICamera {
public:
	ICamera() = default;
	virtual Eigen::Matrix4f view() = 0;
	virtual Eigen::Matrix4f projection() = 0;
	virtual void move(const Eigen::Vector3f& offset) = 0;
	virtual void forward(float step) = 0;
};

class NoCamera : public ICamera {
public:
	NoCamera() = default;
	virtual Eigen::Matrix4f view() override;
	virtual Eigen::Matrix4f projection() override;

	virtual void move(const Eigen::Vector3f& offset) override;
	virtual void forward(float step) override;
};

class ProjectionCamera : public ICamera {
public:
	ProjectionCamera(Eigen::Vector3f position, Eigen::Vector3f target, float aspectRatio = 16.0f / 9.0f,
					float fov = 45.0f, float zNear = 1.0f, float zFar = 1000.0f);
	~ProjectionCamera() = default;
	Eigen::Matrix4f view() override;
	Eigen::Matrix4f projection() override;
	void updateViewMatrix();
	void updateProjectionMatrix();
	void move(const Eigen::Vector3f& offset);
	void forward(float step);

private:
	float m_fov;
	float m_aspectRatio;
	float m_zNear;
	float m_zFar;
	const double M_PI = 3.14159265358979323846;

	Eigen::Vector3f m_position;
	Eigen::Vector3f m_front;
	Eigen::Vector3f m_right;
	Eigen::Vector3f m_y;
	Eigen::Vector3f m_up;
	Eigen::Vector3f m_target;
	bool m_viewDirty;

	Eigen::Matrix4f m_view;
	Eigen::Matrix4f m_projection;

	void copy(Eigen::Matrix4f& mat, DirectX::XMMATRIX& dirMat);
};
