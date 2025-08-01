#include "D3D11Window.h"
#include <iostream>
#include <QKeyEvent>
#include "../Core/DrawScreen.h"

D3D11Window::D3D11Window(DrawMangerBase* manger)
    :m_manger(manger) {

    setMouseTracking(true); // 实时监听鼠标移动（无需按下按键）
    setCursor(Qt::BlankCursor); // 隐藏鼠标指针（可选）
    grabMouse();

    setAttribute(Qt::WA_PaintOnScreen);
    uint32_t _width = this->width();
    uint32_t _height = this->height();
    HWND windowHandle = reinterpret_cast<HWND>(this->winId());
    m_manger->init(windowHandle, _width, _height);
    m_curTime = std::chrono::high_resolution_clock::now();
    m_preTime = std::chrono::high_resolution_clock::now();
}

D3D11Window::~D3D11Window()
{

}

void D3D11Window::paintEvent(QPaintEvent *event)
{
    m_curTime = std::chrono::high_resolution_clock::now();
    long long dtLong = (m_curTime - m_preTime).count();
    double dt = dtLong / (1000.0 * 1000.0);
    m_preTime = m_curTime;
    m_manger->present(dt);
    update();
}

void D3D11Window::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key::Key_W:
            m_manger->forwardCamera(m_manger->getScreenDrawer(), m_manger->getCurCameraGroupIndex(m_manger->getScreenDrawer()),0,0.5f);
            break;
        case Qt::Key::Key_S:
            m_manger->forwardCamera(m_manger->getScreenDrawer(), m_manger->getCurCameraGroupIndex(m_manger->getScreenDrawer()),0,-0.5f);
            break;
        case Qt::Key::Key_A:
            m_manger->rightCamera(m_manger->getScreenDrawer(), m_manger->getCurCameraGroupIndex(m_manger->getScreenDrawer()), 0, 0.5f);
            break;
        case Qt::Key::Key_D:
            m_manger->rightCamera(m_manger->getScreenDrawer(), m_manger->getCurCameraGroupIndex(m_manger->getScreenDrawer()), 0, -0.5f);
            break;
        case Qt::Key::Key_Q:
            m_manger->setCurCameraManager(m_manger->getScreenDrawer(), 1);
            break;
        case Qt::Key::Key_E:
            m_manger->setCurCameraManager(m_manger->getScreenDrawer(), 0);
            break;
        case Qt::Key::Key_Escape:
            close();
            break;
    }
}

void D3D11Window::mousePressEvent(QMouseEvent* event) {
    m_mouseLastPox = event->pos(); // 记录初始鼠标位置
}


void D3D11Window::mouseMoveEvent(QMouseEvent* event) {
    QPoint center = rect().center();

    // 2. 计算鼠标偏移量（用于旋转）
    float dx = event->x() - center.x();
    float dy = event->y() - center.y();

    m_mouseLastPox = event->pos();

    // 调整旋转灵敏度
    float sensitivity = 0.1f;
    dx *= sensitivity;
    dy *= sensitivity;

    m_manger->rotateCamera(m_manger->getScreenDrawer(), m_manger->getCurCameraGroupIndex(m_manger->getScreenDrawer()), 0, dx,dy);

    QCursor::setPos(mapToGlobal(center));
}

void D3D11Window::resizeEvent(QResizeEvent *event)
{

}

QPaintEngine* D3D11Window::paintEngine() const
{
    return 0;
}