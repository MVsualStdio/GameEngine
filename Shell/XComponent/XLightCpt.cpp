#include <rttr/registration>
#include "XLightCpt.h"
#include "../../Core/Component/GameObject.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<XLightCpt>("XLight")
			.constructor<>()(rttr::policy::ctor::as_raw_ptr);
			//.property("m_lightColor", &XLightCpt::getLightColor, &XLightCpt::setLightColor);
}

XLightCpt::XLightCpt() 
	: m_lightColor(Eigen::Vector4f{ 1.0f,1.0f,1.0f,1.0f })
	, m_position(Eigen::Vector3f{ 30.0f,30.0f,-40.0f }){

}