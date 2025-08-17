#include "Transform.h"
#include <rttr/registration>

RTTR_REGISTRATION
{
	rttr::registration::class_<Transform>("Transform")
			.constructor<>()(rttr::policy::ctor::as_raw_ptr)
			.property("m_position", &Transform::getPosition, &Transform::setPosition)
			.property("m_rotation", &Transform::getRotate, &Transform::setRotate)
			.property("m_scale", &Transform::getScale, &Transform::setScale);
}

Transform::Transform()
	: m_position{0,0,0}
	, m_scale(1,1,1)
	, m_rotate(0,0,0) {

}

Eigen::Matrix4f Transform::getMatrix() {

    Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();

    // Apply translation
    matrix.block<3, 1>(0, 3) = m_position;

    // Apply rotation (as Euler angles in radians, XYZ order)
    Eigen::Matrix3f rotation;
    rotation = Eigen::AngleAxisf(m_rotate.x(), Eigen::Vector3f::UnitX())
        * Eigen::AngleAxisf(m_rotate.y(), Eigen::Vector3f::UnitY())
        * Eigen::AngleAxisf(m_rotate.z(), Eigen::Vector3f::UnitZ());
    matrix.block<3, 3>(0, 0) = rotation;

    // Apply scale
    matrix.block<3, 3>(0, 0) *= m_scale.asDiagonal();

	return matrix.transpose();
}