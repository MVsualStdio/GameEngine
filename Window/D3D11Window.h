#pragma once

#include <QtWidgets/QWidget>
#include "../Core/D3D11Context.h"
#include "../Core/DrawManger.h"
#include <chrono>

// Create a QWindow subclass
class D3D11Window : public QWidget {
    Q_OBJECT

public:
    explicit D3D11Window(DrawMangerBase* manger);
    virtual ~D3D11Window();
protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
    QPaintEngine* paintEngine() const;
    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

private:
    DrawMangerBase* m_manger;
    std::chrono::steady_clock::time_point m_preTime;
    std::chrono::steady_clock::time_point m_curTime;

    QPoint m_mouseLastPox;

};


