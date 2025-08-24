#include "LoadScence.h"


Assimp::Importer* LoadScene::importer() {
    static Assimp::Importer gImporter;
    return &gImporter;
}

unsigned int LoadScene::loadFlag() {
    return (aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace);
}

