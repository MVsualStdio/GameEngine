#pragma once

#include <QtWidgets/QWidget>

#include "D3D11Context.h"
#include "DrawManger.h"

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
    DrawMangerBase* m_manger;
};


