#pragma once
#include "Component.h"
#include "Eigen/Core"
#include "Eigen/Dense"

class Transform : public Component {
public:
	Transform();
	~Transform() = default;

	void setPosition(Eigen::Vector3f position) { m_position = position; }
	void setScale(Eigen::Vector3f scale) { m_scale = scale; }
	void setRotate(Eigen::Vector3f rotate) { m_rotate = rotate; }
	void offsetPosition(Eigen::Vector3f offset) { m_position += offset; }

	Eigen::Vector3f getPosition() { return m_position; }
	Eigen::Vector3f getScale() { return m_scale; }
	Eigen::Vector3f getRotate() { return m_rotate; }

	Eigen::Matrix4f getMatrix();

private:
	Eigen::Vector3f m_position;
	Eigen::Vector3f m_scale;
	Eigen::Vector3f m_rotate;

};