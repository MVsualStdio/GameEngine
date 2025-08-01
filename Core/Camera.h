#pragma once
#include "Eigen/Core"
#include "Eigen/Dense"
#include <DirectXMath.h>

class ICamera {
public:
	ICamera() = default;

	virtual const Eigen::Matrix4f view() = 0;
	virtual const Eigen::Matrix4f projection() = 0;

	virtual const Eigen::Vector3f position() = 0;

	virtual void move(const Eigen::Vector3f& offset) = 0;
	virtual void right(float step) = 0;
	virtual void forward(float step) = 0;

	virtual void rotate(float yaw, float pitch) = 0;
};

class NoCamera : public ICamera {
public:
	NoCamera() = default;
	const Eigen::Matrix4f view() override;
	const Eigen::Matrix4f projection() override;
	const Eigen::Vector3f position() override;
	void move(const Eigen::Vector3f& offset) override;
	void forward(float step) override;
	void right(float step) override;
	void rotate(float yaw, float pitch) override;
};

class ProjectionCamera : public ICamera {
public:
	ProjectionCamera(Eigen::Vector3f position, Eigen::Vector3f target, float aspectRatio = 16.0f / 9.0f,
					float fov = 45.0f, float zNear = 1.0f, float zFar = 1000.0f);
	~ProjectionCamera() = default;
	const Eigen::Matrix4f view() override;
	const Eigen::Matrix4f projection() override;
	const Eigen::Vector3f position() override;
	void updateViewMatrix();
	void updateProjectionMatrix();
	void move(const Eigen::Vector3f& offset) override;
	void forward(float step) override;
	void right(float step) override;

	void rotate(float yaw, float pitch) override;
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
