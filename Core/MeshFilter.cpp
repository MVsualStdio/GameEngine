#include "MeshFilter.h"
#include "VertexLayout.h"

std::shared_ptr<AnyVertexBuffer> Geometry::CreateTrangle(Eigen::Vector3f points[3]) {
    std::shared_ptr<VertexBuffer<VertexUV>> pVertex = std::make_shared<VertexUVData>();

    pVertex->vertices.resize(3);
    pVertex->vertices[0].pos = Eigen::Vector3<float>(points[0].x(), points[0].y(), points[0].z());
    pVertex->vertices[1].pos = Eigen::Vector3<float>(points[1].x(), points[1].y(), points[1].z());
    pVertex->vertices[2].pos = Eigen::Vector3<float>(points[2].x(), points[2].y(), points[2].z());

    pVertex->vertices[0].color = Eigen::Vector4<float>(0.0f, 0.0f, 0.0f, 0.0f);
    pVertex->vertices[1].color = Eigen::Vector4<float>(0.0f, 0.0f, 0.0f, 0.0f);
    pVertex->vertices[2].color = Eigen::Vector4<float>(0.0f, 0.0f, 0.0f, 0.0f);

    pVertex->vertices[0].uv = Eigen::Vector2<float>(1.0f, 0.0f);
    pVertex->vertices[1].uv = Eigen::Vector2<float>(0.0f, 1.0f);
    pVertex->vertices[2].uv = Eigen::Vector2<float>(1.0f, 1.0f);

    pVertex->vertices[0].normal = Eigen::Vector3<float>(0.0f, 0.0f, 0.0f);
    pVertex->vertices[1].normal = Eigen::Vector3<float>(0.0f, 0.0f, 0.0f);
    pVertex->vertices[2].normal = Eigen::Vector3<float>(0.0f, 0.0f, 0.0f);

    pVertex->indices = { 0,1,2 };

    return std::make_shared<AnyVertexBuffer>(pVertex);
}

std::shared_ptr<AnyVertexBuffer> Geometry::CreateCube(float width, float height, float depth) {
    std::shared_ptr<VertexBuffer<VertexUV>> pVertex = std::make_shared<VertexUVData>();
    pVertex->vertices.resize(24);

    float w2 = width / 2, h2 = height / 2, d2 = depth / 2;

    pVertex->vertices[0].pos = Eigen::Vector3<float>(w2, -h2, -d2);
    pVertex->vertices[1].pos = Eigen::Vector3<float>(w2, h2, -d2);
    pVertex->vertices[2].pos = Eigen::Vector3<float>(w2, h2, d2);
    pVertex->vertices[3].pos = Eigen::Vector3<float>(w2, -h2, d2);

    pVertex->vertices[4].pos = Eigen::Vector3<float>(-w2, -h2, -d2);
    pVertex->vertices[5].pos = Eigen::Vector3<float>(-w2, h2, -d2);
    pVertex->vertices[6].pos = Eigen::Vector3<float>(-w2, h2, d2);
    pVertex->vertices[7].pos = Eigen::Vector3<float>(-w2, -h2, d2);

    pVertex->vertices[8].pos = Eigen::Vector3<float>(-w2, h2, -d2);
    pVertex->vertices[9].pos = Eigen::Vector3<float>(-w2, h2, d2);
    pVertex->vertices[10].pos = Eigen::Vector3<float>(w2, h2, d2);
    pVertex->vertices[11].pos = Eigen::Vector3<float>(w2, h2, -d2);

    pVertex->vertices[12].pos = Eigen::Vector3<float>(w2, -h2, -d2);
    pVertex->vertices[13].pos = Eigen::Vector3<float>(w2, -h2, d2);
    pVertex->vertices[14].pos = Eigen::Vector3<float>(-w2, -h2, d2);
    pVertex->vertices[15].pos = Eigen::Vector3<float>(-w2, -h2, -d2);

    pVertex->vertices[16].pos = Eigen::Vector3<float>(w2, -h2, d2);
    pVertex->vertices[17].pos = Eigen::Vector3<float>(w2, h2, d2);
    pVertex->vertices[18].pos = Eigen::Vector3<float>(-w2, h2, d2);
    pVertex->vertices[19].pos = Eigen::Vector3<float>(-w2, -h2, d2);

    pVertex->vertices[20].pos = Eigen::Vector3<float>(-w2, -h2, -d2);
    pVertex->vertices[21].pos = Eigen::Vector3<float>(-w2, h2, -d2);
    pVertex->vertices[22].pos = Eigen::Vector3<float>(w2, h2, -d2);
    pVertex->vertices[23].pos = Eigen::Vector3<float>(w2, -h2, -d2);


    for (size_t i = 0; i < 4; ++i)
    {
        // 右面(+X面)
        pVertex->vertices[i].normal = Eigen::Vector3<float>(1.0f, 0.0f, 0.0f);
        //geoData.tangents[i] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
        // 左面(-X面)
        pVertex->vertices[i + 4].normal = Eigen::Vector3<float>(-1.0f, 0.0f, 0.0f);
        //geoData.tangents[i + 4] = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
        // 顶面(+Y面)
        pVertex->vertices[i + 8].normal = Eigen::Vector3<float>(0.0f, 1.0f, 0.0f);
        // geoData.tangents[i + 8] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
         // 底面(-Y面)
        pVertex->vertices[i + 12].normal = Eigen::Vector3<float>(0.0f, -1.0f, 0.0f);
        //geoData.tangents[i + 12] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // 背面(+Z面)
        pVertex->vertices[i + 16].normal = Eigen::Vector3<float>(0.0f, 0.0f, 1.0f);
        //geoData.tangents[i + 16] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // 正面(-Z面)
        pVertex->vertices[i + 20].normal = Eigen::Vector3<float>(0.0f, 0.0f, -1.0f);
        //geoData.tangents[i + 20] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    for (size_t i = 0; i < 6; ++i)
    {
        pVertex->vertices[i * 4].uv = Eigen::Vector2<float>(0.0f, 1.0f);
        pVertex->vertices[i * 4 + 1].uv = Eigen::Vector2<float>(0.0f, 0.0f);
        pVertex->vertices[i * 4 + 2].uv = Eigen::Vector2<float>(1.0f, 0.0f);
        pVertex->vertices[i * 4 + 3].uv = Eigen::Vector2<float>(1.0f, 1.0f);
    }

    pVertex->indices = {
        0, 1, 2, 2, 3, 0,		// 右面(+X面)
        6, 5, 4, 4, 7, 6,		// 左面(-X面)
        8, 9, 10, 10, 11, 8,	// 顶面(+Y面)
        12, 13, 14, 14, 15, 12,	// 底面(-Y面)
        16, 17, 18, 18, 19, 16, // 背面(+Z面)
        20, 21, 22, 22, 23, 20	// 正面(-Z面)
    };
    return std::make_shared<AnyVertexBuffer>(pVertex);
}

std::shared_ptr<AnyVertexBuffer> Geometry::CreateCube(float width, float height, float depth, Eigen::Vector4f color) {
    std::shared_ptr<VertexBuffer<VertexUV>> pVertex = std::make_shared<VertexUVData>();
    pVertex->vertices.resize(24);

    float w2 = width / 2, h2 = height / 2, d2 = depth / 2;

    pVertex->vertices[0].pos = Eigen::Vector3<float>(w2, -h2, -d2);
    pVertex->vertices[1].pos = Eigen::Vector3<float>(w2, h2, -d2);
    pVertex->vertices[2].pos = Eigen::Vector3<float>(w2, h2, d2);
    pVertex->vertices[3].pos = Eigen::Vector3<float>(w2, -h2, d2);

    pVertex->vertices[4].pos = Eigen::Vector3<float>(-w2, -h2, -d2);
    pVertex->vertices[5].pos = Eigen::Vector3<float>(-w2, h2, -d2);
    pVertex->vertices[6].pos = Eigen::Vector3<float>(-w2, h2, d2);
    pVertex->vertices[7].pos = Eigen::Vector3<float>(-w2, -h2, d2);

    pVertex->vertices[8].pos = Eigen::Vector3<float>(-w2, h2, -d2);
    pVertex->vertices[9].pos = Eigen::Vector3<float>(-w2, h2, d2);
    pVertex->vertices[10].pos = Eigen::Vector3<float>(w2, h2, d2);
    pVertex->vertices[11].pos = Eigen::Vector3<float>(w2, h2, -d2);

    pVertex->vertices[12].pos = Eigen::Vector3<float>(w2, -h2, -d2);
    pVertex->vertices[13].pos = Eigen::Vector3<float>(w2, -h2, d2);
    pVertex->vertices[14].pos = Eigen::Vector3<float>(-w2, -h2, d2);
    pVertex->vertices[15].pos = Eigen::Vector3<float>(-w2, -h2, -d2);

    pVertex->vertices[16].pos = Eigen::Vector3<float>(w2, -h2, d2);
    pVertex->vertices[17].pos = Eigen::Vector3<float>(w2, h2, d2);
    pVertex->vertices[18].pos = Eigen::Vector3<float>(-w2, h2, d2);
    pVertex->vertices[19].pos = Eigen::Vector3<float>(-w2, -h2, d2);

    pVertex->vertices[20].pos = Eigen::Vector3<float>(-w2, -h2, -d2);
    pVertex->vertices[21].pos = Eigen::Vector3<float>(-w2, h2, -d2);
    pVertex->vertices[22].pos = Eigen::Vector3<float>(w2, h2, -d2);
    pVertex->vertices[23].pos = Eigen::Vector3<float>(w2, -h2, -d2);


    for (size_t i = 0; i < 4; ++i)
    {
        // 右面(+X面)
        pVertex->vertices[i].normal = Eigen::Vector3<float>(1.0f, 0.0f, 0.0f);
        //geoData.tangents[i] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
        // 左面(-X面)
        pVertex->vertices[i + 4].normal = Eigen::Vector3<float>(-1.0f, 0.0f, 0.0f);
        //geoData.tangents[i + 4] = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
        // 顶面(+Y面)
        pVertex->vertices[i + 8].normal = Eigen::Vector3<float>(0.0f, 1.0f, 0.0f);
        // geoData.tangents[i + 8] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
         // 底面(-Y面)
        pVertex->vertices[i + 12].normal = Eigen::Vector3<float>(0.0f, -1.0f, 0.0f);
        //geoData.tangents[i + 12] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // 背面(+Z面)
        pVertex->vertices[i + 16].normal = Eigen::Vector3<float>(0.0f, 0.0f, 1.0f);
        //geoData.tangents[i + 16] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // 正面(-Z面)
        pVertex->vertices[i + 20].normal = Eigen::Vector3<float>(0.0f, 0.0f, -1.0f);
        //geoData.tangents[i + 20] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    for (size_t i = 0; i < 6; ++i)
    {
        pVertex->vertices[i * 4].uv = Eigen::Vector2<float>(0.0f, 1.0f);
        pVertex->vertices[i * 4 + 1].uv = Eigen::Vector2<float>(0.0f, 0.0f);
        pVertex->vertices[i * 4 + 2].uv = Eigen::Vector2<float>(1.0f, 0.0f);
        pVertex->vertices[i * 4 + 3].uv = Eigen::Vector2<float>(1.0f, 1.0f);
    }

    for (size_t i = 0; i < 24; ++i) {
        pVertex->vertices[i].color = color;
    }

    pVertex->indices = {
        0, 1, 2, 2, 3, 0,		// 右面(+X面)
        6, 5, 4, 4, 7, 6,		// 左面(-X面)
        8, 9, 10, 10, 11, 8,	// 顶面(+Y面)
        12, 13, 14, 14, 15, 12,	// 底面(-Y面)
        16, 17, 18, 18, 19, 16, // 背面(+Z面)
        20, 21, 22, 22, 23, 20	// 正面(-Z面)
    };
    return std::make_shared<AnyVertexBuffer>(pVertex);
}

std::shared_ptr<AnyVertexBuffer> Geometry::CreatePlane(float width, float depth, float texU, float texV) {
    std::shared_ptr<VertexBuffer<VertexUV>> pVertex = std::make_shared<VertexUVData>();
    pVertex->vertices.resize(4);

    uint32_t vIndex = 0;
    pVertex->vertices[vIndex].pos = Eigen::Vector3<float>(-width / 2, 0.0f, -depth / 2);
    pVertex->vertices[vIndex].normal = Eigen::Vector3<float>(0.0f, 1.0f, 0.0f);
    pVertex->vertices[vIndex].color = Eigen::Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f);
    pVertex->vertices[vIndex++].uv = Eigen::Vector2<float>(0.0f, texV);

    pVertex->vertices[vIndex].pos = Eigen::Vector3<float>(-width / 2, 0.0f, depth / 2);
    pVertex->vertices[vIndex].normal = Eigen::Vector3<float>(0.0f, 1.0f, 0.0f);
    pVertex->vertices[vIndex].color = Eigen::Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f);
    pVertex->vertices[vIndex++].uv = Eigen::Vector2<float>(0.0f, 0.0f);

    pVertex->vertices[vIndex].pos = Eigen::Vector3<float>(width / 2, 0.0f, depth / 2);
    pVertex->vertices[vIndex].normal = Eigen::Vector3<float>(0.0f, 1.0f, 0.0f);
    pVertex->vertices[vIndex].color = Eigen::Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f);
    pVertex->vertices[vIndex++].uv = Eigen::Vector2<float>(texU, 0.0f);

    pVertex->vertices[vIndex].pos = Eigen::Vector3<float>(width / 2, 0.0f, -depth / 2);
    pVertex->vertices[vIndex].normal = Eigen::Vector3<float>(0.0f, 1.0f, 0.0f);
    pVertex->vertices[vIndex].color = Eigen::Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f);
    pVertex->vertices[vIndex++].uv = Eigen::Vector2<float>(texU, texV);

    pVertex->indices = { 0, 1, 2, 2, 3, 0 };

    return std::make_shared<AnyVertexBuffer>(pVertex);
}

std::unordered_map<std::string, std::vector<MeshFilter::MeshInfo>> MeshFilter::gMeshs;

std::vector<MeshFilter::MeshInfo> MeshFilter::getMeshs(std::string path) {
    if (gMeshs.find(path) != gMeshs.end()) {
        return gMeshs[path];
    }
    if (loadScene(path)) {
        MeshFilter::getMeshs(path);
    }
}

MeshFilter* MeshFilter::instance() {
    static MeshFilter gMeshFilter;
    return &gMeshFilter;
}

bool MeshFilter::loadScene(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        return false;
    }

    // process ASSIMP's root node recursively
    return processNode(path, scene->mRootNode, scene);
    
}

bool MeshFilter::processNode(std::string& key, aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        gMeshs[key].push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(key, node->mChildren[i], scene);
    }
    return true;
}

MeshFilter::MeshInfo MeshFilter::processMesh(aiMesh* mesh, const aiScene* scene) {

}