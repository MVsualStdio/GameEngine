#include "Camera.h"


ProjectionCamera::ProjectionCamera(Eigen::Vector3f position, Eigen::Vector3f target, float aspectRatio,
    float fov, float zNear, float zFar)
    : m_position(position)
    , m_target(target)
    , m_aspectRatio(aspectRatio)
    , m_fov(fov)
    , m_zNear(zNear)
    , m_zFar(zFar)
    , m_viewDirty(true)
    , m_y(Eigen::Vector3f(0,1,0)) {

    m_front = (m_position - m_target).normalized();
    m_right = m_front.cross(m_y).normalized();
    m_up = m_right.cross(m_front).normalized();

    updateViewMatrix();
    updateProjectionMatrix();
}

Eigen::Matrix4f ProjectionCamera::view() {
    updateViewMatrix();
    return m_view;
}

Eigen::Matrix4f ProjectionCamera::projection() {
    return m_projection;
}

void ProjectionCamera::updateViewMatrix()
{
    if (m_viewDirty) {
        m_view = Eigen::Matrix4f::Identity();

        Eigen::Vector3f f = m_front;
        Eigen::Vector3f s = m_right;
        Eigen::Vector3f u = m_up;

        m_view(0, 0) = s.x();
        m_view(0, 1) = s.y();
        m_view(0, 2) = s.z();
        m_view(1, 0) = u.x();
        m_view(1, 1) = u.y();
        m_view(1, 2) = u.z();
        m_view(2, 0) = -f.x();
        m_view(2, 1) = -f.y();
        m_view(2, 2) = -f.z();

        m_view(3, 0) = -s.dot(m_position);
        m_view(3, 1) = -u.dot(m_position);
        m_view(3, 2) = f.dot(m_position);

        

        //DirectX::XMMATRIX dxView = DirectX::XMMatrixLookAtLH(
        //    DirectX::XMVectorSet(m_position.x(), m_position.y(), m_position.z(), 0.0f),
        //    DirectX::XMVectorSet(m_target.x(), m_target.y(), m_target.z(), 0.0f),
        //    DirectX::XMVectorSet(m_y.x(), m_y.y(), m_y.z(), 0.0f)
        //);
        //copy(m_view, dxView);
        m_viewDirty = false;
    }
}


void ProjectionCamera::updateProjectionMatrix() {
    float angle = m_fov * M_PI / 180.0f;

    float tanHalfFovy = tan(angle / 2.0f);

    m_projection = Eigen::Matrix4f::Zero();
    m_projection(0, 0) = 1.0f / (m_aspectRatio * tanHalfFovy);
    m_projection(1, 1) = 1.0f / tanHalfFovy;
    m_projection(2, 2) = (m_zFar + m_zNear) / (m_zFar - m_zNear);
    m_projection(3, 2) = -1.0f;
    m_projection(2, 3) = (m_zFar * m_zNear) / (m_zFar - m_zNear);

    //DirectX::XMMATRIX dxPro = DirectX::XMMatrixPerspectiveFovLH(angle, m_aspectRatio, m_zNear, m_zFar);
    //copy(m_projection, dxPro);
}

void ProjectionCamera::copy(Eigen::Matrix4f& mat, DirectX::XMMATRIX& dxMat) {
    DirectX::XMFLOAT4X4 dxFloat4x4;
    DirectX::XMStoreFloat4x4(&dxFloat4x4, dxMat);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            mat(i, j) = dxFloat4x4.m[i][j];
        }
    }
}

void ProjectionCamera::move(const Eigen::Vector3f& offset) {
    m_position += offset;
    m_viewDirty = true;
}

void ProjectionCamera::forward(float step) {
    m_position = m_position - m_front * step;
    m_right = m_front.cross(m_y).normalized();
    m_up = m_right.cross(m_front).normalized();
    m_viewDirty = true;
}