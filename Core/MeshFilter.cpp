#include "MeshFilter.h"
#include "Animation/KeyAnimation.h"

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
        // ����(+X��)
        pVertex->vertices[i].normal = Eigen::Vector3<float>(1.0f, 0.0f, 0.0f);
        //geoData.tangents[i] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
        // ����(-X��)
        pVertex->vertices[i + 4].normal = Eigen::Vector3<float>(-1.0f, 0.0f, 0.0f);
        //geoData.tangents[i + 4] = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
        // ����(+Y��)
        pVertex->vertices[i + 8].normal = Eigen::Vector3<float>(0.0f, 1.0f, 0.0f);
        // geoData.tangents[i + 8] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
         // ����(-Y��)
        pVertex->vertices[i + 12].normal = Eigen::Vector3<float>(0.0f, -1.0f, 0.0f);
        //geoData.tangents[i + 12] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // ����(+Z��)
        pVertex->vertices[i + 16].normal = Eigen::Vector3<float>(0.0f, 0.0f, 1.0f);
        //geoData.tangents[i + 16] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // ����(-Z��)
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
        0, 1, 2, 2, 3, 0,		// ����(+X��)
        6, 5, 4, 4, 7, 6,		// ����(-X��)
        8, 9, 10, 10, 11, 8,	// ����(+Y��)
        12, 13, 14, 14, 15, 12,	// ����(-Y��)
        16, 17, 18, 18, 19, 16, // ����(+Z��)
        20, 21, 22, 22, 23, 20	// ����(-Z��)
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
        // ����(+X��)
        pVertex->vertices[i].normal = Eigen::Vector3<float>(1.0f, 0.0f, 0.0f);
        //geoData.tangents[i] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
        // ����(-X��)
        pVertex->vertices[i + 4].normal = Eigen::Vector3<float>(-1.0f, 0.0f, 0.0f);
        //geoData.tangents[i + 4] = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
        // ����(+Y��)
        pVertex->vertices[i + 8].normal = Eigen::Vector3<float>(0.0f, 1.0f, 0.0f);
        // geoData.tangents[i + 8] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
         // ����(-Y��)
        pVertex->vertices[i + 12].normal = Eigen::Vector3<float>(0.0f, -1.0f, 0.0f);
        //geoData.tangents[i + 12] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // ����(+Z��)
        pVertex->vertices[i + 16].normal = Eigen::Vector3<float>(0.0f, 0.0f, 1.0f);
        //geoData.tangents[i + 16] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // ����(-Z��)
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
        0, 1, 2, 2, 3, 0,		// ����(+X��)
        6, 5, 4, 4, 7, 6,		// ����(-X��)
        8, 9, 10, 10, 11, 8,	// ����(+Y��)
        12, 13, 14, 14, 15, 12,	// ����(-Y��)
        16, 17, 18, 18, 19, 16, // ����(+Z��)
        20, 21, 22, 22, 23, 20	// ����(-Z��)
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

std::unordered_map<std::string, Model> MeshFilter::gMeshs;

std::vector<MeshInfo> MeshFilter::getMeshInfo(std::string path) {
    if (gMeshs.find(path) != gMeshs.end()) {
        return gMeshs[path].mesh;
    }
    const aiScene* scene = LoadScene::importer()->ReadFile(path, LoadScene::loadFlag());

    if (scene && scene->mRootNode) {
        processNode(path, scene->mRootNode, scene);
        return MeshFilter::getMeshInfo(path);
    }
    return std::vector<MeshInfo>();
}

Model MeshFilter::getModel(std::string path) {
    if (gMeshs.find(path) != gMeshs.end()) {
        return gMeshs[path];
    }
    else {
        getMeshInfo(path);
        return getModel(path);
    }
}

MeshFilter* MeshFilter::instance() {
    static MeshFilter gMeshFilter;
    return &gMeshFilter;
}

#include <iostream>
bool MeshFilter::processNode(std::string& key, aiNode* node, const aiScene* scene) {
    const char* name = node->mName.C_Str();
    std::cout << name << " : " << node->mNumMeshes << std::endl;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(key, mesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(key, node->mChildren[i], scene);
    }
    return true;
}

void MeshFilter::processMesh(std::string& key, aiMesh* mesh, const aiScene* scene) {
    std::shared_ptr<VertexBuffer<VertexUV>> pVertex = std::make_shared<VertexUVData>();
    pVertex->vertices.resize(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        pVertex->vertices[i].pos = { mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z };
        if (mesh->HasNormals()) {
            pVertex->vertices[i].normal = { mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z };
        }
        if (mesh->mTextureCoords[0]) {
            pVertex->vertices[i].uv = { mesh->mTextureCoords[0][i].x , mesh->mTextureCoords[0][i].y };
        }
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            pVertex->indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::shared_ptr<Texture2D> textures;

    std::shared_ptr<AnyVertexBuffer> buffer = std::make_shared<AnyVertexBuffer>(pVertex);
    gMeshs[key].mesh.push_back({ buffer, textures });
    
    extractBoneWeightForVertices(key, buffer, mesh, scene);
}

static inline Eigen::Matrix4f ConvertMatrixToEigenFormat(const aiMatrix4x4& from)
{
    return Eigen::Matrix4f{
       {from.a1, from.b1, from.c1, from.d1},
       {from.a2, from.b2, from.c2, from.d2},
       {from.a3, from.b3, from.c3, from.d3},
       {from.a4, from.b4, from.c4, from.d4}
    };
}

void MeshFilter::extractBoneWeightForVertices(std::string& key, std::shared_ptr<AnyVertexBuffer> buffer, aiMesh* mesh, const aiScene* scene) {
    std::unordered_map<std::string, BoneInfo>& bones = gMeshs[key].bone;
    int& boneCount = gMeshs[key].boneCount;
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (bones.find(boneName) == bones.end()) {
            BoneInfo newBoneInfo;
            newBoneInfo.id = boneCount;
            newBoneInfo.offset = ConvertMatrixToEigenFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
            bones[boneName] = newBoneInfo;
            boneID = boneCount;
            boneCount++;
        }
        else {
            boneID = bones[boneName].id;
        }

        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            std::shared_ptr<VertexBuffer<VertexUV>> vertex = buffer->vertex<VertexUV>();
            vertex->bonesWidget.push_back(boneVertex{ vertexId,boneID, weight });
        }
    }
}