#include "D3D11Window.h"
#include <QtWidgets/QApplication>
#include "Scene.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DrawMangerTexture base;
    D3D11Window window(&base);
    
    window.show();

    return app.exec();
}
