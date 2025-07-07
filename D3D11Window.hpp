#ifndef D3D11_WINDOW_H
#define D3D11_WINDOW_H

#include <QtWidgets/QWidget>

#include "D3D11Context.hpp"

// Create a QWindow subclass
class D3D11Window : public QWidget {
    Q_OBJECT

public:
    explicit D3D11Window();
    virtual ~D3D11Window();
protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
    QPaintEngine* paintEngine() const;
private:
    D3D11Context* m_Context;
};

#endif // OPENGLWINDOW_H

