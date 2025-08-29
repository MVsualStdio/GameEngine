#pragma once
#include <Eigen/Core>
#include "../../Core/Component/Component.h"
#include "../../Core/Component/Transform.h"

class XLightCpt : public Component {
public:
	XLightCpt(bool isComponent = true);
	~XLightCpt() = default;

	Eigen::Vector4f getLightColor() { return m_lightColor; }
	void setLightColor(Eigen::Vector4f lightColor) { m_lightColor = lightColor; }

	Eigen::Vector3f getPosition() { return m_position; }
	void setPosition(Eigen::Vector3f position) { m_position = position; }

	Eigen::Vector3f getLookAt() { return m_lookAt; }
	void setLookAt(Eigen::Vector3f lookAt) { m_lookAt = lookAt; }

private:
	Eigen::Vector3f m_position;
	Eigen::Vector4f m_lightColor;
	Eigen::Vector3f m_lookAt;
};