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

Eigen::Matrix4f Assimp2Eigen::ConvertMatrixToEigenFormat(const aiMatrix4x4& from)
{
    return Eigen::Matrix4f{
       {from.a1, from.b1, from.c1, from.d1},
       {from.a2, from.b2, from.c2, from.d2},
       {from.a3, from.b3, from.c3, from.d3},
       {from.a4, from.b4, from.c4, from.d4}
    };
}
