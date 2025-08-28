#include "Window/D3D11Window.h"
#include <QtWidgets/QApplication>
#include "Core/Scene.h"
#include "Shell/DrawScreenManager.h"
#include "Shell/XWoodCrateBox.h"
#include "Core/Component/GameObject.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);


    DrawMangerScreen base;
    //DrawMangerTexture base;
    D3D11Window window(&base);
    
    window.show();

    return app.exec();
}
