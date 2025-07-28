#include "D3D11Window.h"
#include <iostream>
#include <QKeyEvent>

D3D11Window::D3D11Window(DrawMangerBase* manger)
    :m_manger(manger) {
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
            m_manger->moveMainCamera(0, 0, 0.5f);
            break;
        case Qt::Key::Key_S:
            m_manger->moveMainCamera(0, 0, -0.5f);
            break;
    }
}

void D3D11Window::resizeEvent(QResizeEvent *event)
{

}

QPaintEngine* D3D11Window::paintEngine() const
{
    return 0;
}