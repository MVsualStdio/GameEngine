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

	Eigen::Matrix4f world = Eigen::Matrix4f::Identity();
	//world.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_rotate).toRotationMatrix();
	
	world(0, 0) *= m_scale.x();
	world(1, 1) *= m_scale.y();
	world(2, 2) *= m_scale.z();
	world(3, 0) = m_position.x();
	world(3, 1) = m_position.y();
	world(3, 2) = m_position.z();

	return world;
}