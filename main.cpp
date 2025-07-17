#include "D3D11Window.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    D3D11Window window;
    window.show();

    return app.exec();
}
