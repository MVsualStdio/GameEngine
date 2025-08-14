#include "Camera.h"
#include "Component/GameObject.h"
#include "Component/Transform.h"
#include <rttr/registration>

RTTR_REGISTRATION
{
    rttr::registration::class_<Camera>("Camera")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

std::vector<Camera*> Camera::gAllcamera;

std::vector<Camera*> Camera::CameraList() {
    return gAllcamera;
}

Camera::Camera() {
    gAllcamera.push_back(this);
    m_render = true;
}

void Camera::setProjection(Eigen::Vector3f target, float aspectRatio,
    float fov, float zNear, float zFar) {
    m_target = target;
    m_aspectRatio = aspectRatio;
    m_fov = fov;
    m_zNear = zNear;
    m_zFar = zFar;
    m_viewDirty = true;
    m_y = Eigen::Vector3f(0, 1, 0);

    Transform* transform = dynamic_cast<Transform*>(gameObject()->getComponent("Transform"));
    if (!transform) {
        return;
    }

    m_front = (m_target - transform->getPosition()).normalized();
    updateCoord();
    updateViewMatrix();
    updateProjectionMatrix();
}

void Camera::updateCoord() {
    m_right = -m_front.cross(m_y).normalized();
    m_up = -m_right.cross(m_front).normalized();
    m_viewDirty = true;
}

const Eigen::Matrix4f Camera::view() {
    updateViewMatrix();
    return m_view;
}

const Eigen::Matrix4f Camera::projection() {
    return m_projection;
}

void Camera::updateViewMatrix()
{
     {
        m_view = Eigen::Matrix4f::Identity();

        Transform* transform = dynamic_cast<Transform*>(gameObject()->getComponent("Transform"));
        Eigen::Vector3f position = transform->getPosition();
     
        Eigen::Vector3f f = m_front;
        Eigen::Vector3f s = m_right;
        Eigen::Vector3f u = m_up;

        m_view(0, 0) = s.x();
        m_view(0, 1) = s.y();
        m_view(0, 2) = s.z();
        m_view(1, 0) = u.x();
        m_view(1, 1) = u.y();
        m_view(1, 2) = u.z();
        m_view(2, 0) = f.x();
        m_view(2, 1) = f.y();
        m_view(2, 2) = f.z();

        m_view(0, 3) = -s.dot(position);
        m_view(1, 3) = -u.dot(position);
        m_view(2, 3) = -f.dot(position);

        m_view.transposeInPlace();

        m_viewDirty = false;
    }
}

void Camera::updateProjectionMatrix() {
    float angle = m_fov * M_PI / 180.0f;

    float tanHalfFovy = tan(angle / 2.0f);

    m_projection = Eigen::Matrix4f::Zero();
    m_projection(0, 0) = 1.0f / (m_aspectRatio * tanHalfFovy);
    m_projection(1, 1) = 1.0f / tanHalfFovy;
    m_projection(2, 2) = (m_zFar + m_zNear) / (m_zFar - m_zNear);
    m_projection(2, 3) = 1.0f;
    m_projection(3, 2) = -(m_zFar * m_zNear) / (m_zFar - m_zNear);
}

void Camera::copy(Eigen::Matrix4f& mat, DirectX::XMMATRIX& dxMat) {
    DirectX::XMFLOAT4X4 dxFloat4x4;
    DirectX::XMStoreFloat4x4(&dxFloat4x4, dxMat);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            mat(i, j) = dxFloat4x4.m[i][j];
        }
    }
}