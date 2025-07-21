#include "D3D11Window.h"

D3D11Window::D3D11Window()
{
    setAttribute(Qt::WA_PaintOnScreen);
    uint32_t _width = this->width();
    uint32_t _height = this->height();
    HWND windowHandle = reinterpret_cast<HWND>(this->winId());
    //m_manger = new DrawMangerBaseTrangle();
    m_manger = new DrawMangerTexture();
    m_manger->init(windowHandle, _width, _height);
}

D3D11Window::~D3D11Window()
{
    delete m_manger;
}

void D3D11Window::paintEvent(QPaintEvent *event)
{
    m_manger->present(1.0);
    //update();
}

void D3D11Window::resizeEvent(QResizeEvent *event)
{

}

QPaintEngine* D3D11Window::paintEngine() const
{
    return 0;
}