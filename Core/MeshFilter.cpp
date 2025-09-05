#include "MeshFilter.h"
#include "VertexLayout.h"
#include "TextureManager.h"

LoadMesh::LoadMesh(std::string path) {
    m_rootPath = path + "/../";
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
    //std::shared_ptr<VertexBuffer<VertexUV>> pVertex = std::make_shared<VertexUVData>();
    std::shared_ptr<VertexBuffer<VertexAnimation>> pVertex = std::make_shared<VertexAnimationData>();
    NodeMesh res = NodeMesh();
    res.name = mesh->mName.data;
    res.vertexs = pVertex;
   
    bool hasBone = mesh->mNumBones > 0;
    res.hasBone = hasBone;
    bool hasMorph = mesh->mNumAnimMeshes > 0;
    res.hasMorph = hasMorph;

    pVertex->vertices.resize(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        pVertex->vertices[i].pos = { mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z };
        if (mesh->HasNormals()) {
            pVertex->vertices[i].normal = { mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z };
        }
        if (mesh->mTextureCoords[0]) {
            pVertex->vertices[i].uv = { mesh->mTextureCoords[0][i].x , mesh->mTextureCoords[0][i].y };
        }
        if (mesh->HasVertexColors(0)) {
            aiColor4D* color = mesh->mColors[0]; // 获取第一个颜色集的指针
            if (color) {
                // RGBA格式，值范围通常是0.0-1.0
                pVertex->vertices[i].color = {
                    color[i].r,
                    color[i].g,
                    color[i].b,
                    color[i].a
                };
            }
        }
        else {
            // 设置默认颜色（白色）
            pVertex->vertices[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            pVertex->indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::shared_ptr<Texture2D> textures;
    
    loadMaterialTextures(res.textures, material, aiTextureType_DIFFUSE, NodeMesh::TextureMesh::TextureType::diffuse);
    loadMaterialTextures(res.textures, material, aiTextureType_SPECULAR, NodeMesh::TextureMesh::TextureType::specular);
    loadMaterialTextures(res.textures, material, aiTextureType_HEIGHT, NodeMesh::TextureMesh::TextureType::normal);
    loadMaterialTextures(res.textures, material, aiTextureType_AMBIENT, NodeMesh::TextureMesh::TextureType::height);

    if (hasBone) {
        loadBone(res, mesh, scene);
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            pVertex->vertices[i].boneIndex = { res.bones[i].indexs[0], 0, 0, 0 };
            pVertex->vertices[i].widget = { res.bones[i].weights[0], 0.0, 0.0, 0.0 };
        }
    }

    if (hasMorph) {
        loadMeshMorph(res, mesh, scene);
    }
   

    return res;
}

int LoadMesh::loadBone(NodeMesh& nodeMesh, aiMesh* mesh, const aiScene* scene) {
    nodeMesh.bones.resize(nodeMesh.vertexs->vertices.size());
    
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
        
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();

        Eigen::Matrix4f offset = Assimp2Eigen::ConvertMatrixToEigenFormat(mesh->mBones[boneIndex]->mOffsetMatrix);

        nodeMesh.boneIndexMap[boneName] = { boneIndex,offset };
        
        aiVertexWeight* weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            nodeMesh.bones[vertexId].addBone(boneIndex, boneName, weight);
           
        }
    }
    return mesh->mNumBones;
}

void LoadMesh::loadMaterialTextures(std::vector<NodeMesh::TextureMesh>& textures, aiMaterial* mat, aiTextureType type, NodeMesh::TextureMesh::TextureType textype) {
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        NodeMesh::TextureMesh texture;
        texture.type = textype;
        texture.path = m_rootPath + std::string(str.C_Str());
        textures.push_back(texture);
        TextureManager::instance()->addTexture(texture.path);
    }
}

void LoadMesh::loadMeshMorph(NodeMesh& pVertex, aiMesh* mesh, const aiScene* scene) {
    pVertex.meshAnims.resize(mesh->mNumAnimMeshes);
    for (unsigned int i = 0; i < mesh->mNumAnimMeshes; i++) {
        aiAnimMesh* animMesh = mesh->mAnimMeshes[i];
        pVertex.meshAnims[i].pos.resize(animMesh->mNumVertices);
        pVertex.meshAnims[i].normal.resize(animMesh->mNumVertices);
        for (unsigned int k = 0; k < animMesh->mNumVertices; k++) {
            pVertex.meshAnims[i].pos[k] = Eigen::Vector3f(animMesh->mVertices[k].x, animMesh->mVertices[k].y, animMesh->mVertices[k].z);
            if (animMesh->HasNormals()) {
                pVertex.meshAnims[i].normal[k] = Eigen::Vector3f(animMesh->mNormals[k].x, animMesh->mNormals[k].y, animMesh->mNormals[k].z);
            }
        }
    }
}

MeshFilter* MeshFilter::instance() {
    static MeshFilter gMeshFilter;
    return &gMeshFilter;
}

