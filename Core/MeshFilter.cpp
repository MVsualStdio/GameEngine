#include "MeshFilter.h"
#include "VertexLayout.h"


LoadMesh::LoadMesh(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate 
        | aiProcess_GenSmoothNormals 
        | aiProcess_FlipUVs 
        | aiProcess_CalcTangentSpace);
    if (scene) {
        loadNode(scene->mRootNode, scene);
    }
}

void LoadMesh::loadNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::string name = node->mName.data;
        m_nodeMeshMap[name] = loadMesh(mesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        loadNode(node->mChildren[i], scene);
    }
}

NodeMesh LoadMesh::loadMesh(aiMesh* mesh, const aiScene* scene) {
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

    return NodeMesh{ pVertex , textures };
}

MeshFilter* MeshFilter::instance() {
    static MeshFilter gMeshFilter;
    return &gMeshFilter;
}
