#include "D3D11Window.h"
#include <QtWidgets/QApplication>
#include "Scene.h"
#include "DrawScreenManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DrawMangerScreen base;
    //DrawMangerTexture base;
    D3D11Window window(&base);
    
    window.show();

    return app.exec();
}
