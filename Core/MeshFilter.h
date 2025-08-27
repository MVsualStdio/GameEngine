#pragma once 

#include "D3D11Context.h"
#include "IDrawer.h"
#include "VertexLayout.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Geometry {
public:
	static std::shared_ptr<AnyVertexBuffer> CreateCube(float width, float height, float depth);
	static std::shared_ptr<AnyVertexBuffer> CreateTrangle(Eigen::Vector3f points[3]);
	static std::shared_ptr<AnyVertexBuffer> CreatePlane(float width, float depth, float texU, float texV);
	static std::shared_ptr<AnyVertexBuffer> CreateCube(float width, float height, float depth, Eigen::Vector4f color);
};

struct NodeMesh {
	std::shared_ptr<VertexBuffer<VertexUV>> vertexs;
	std::shared_ptr<Texture2D> textures;
};

class LoadMesh {
public:
	using NodeMeshMap = std::unordered_map<std::string, NodeMesh>;
	LoadMesh(std::string path);
	~LoadMesh() = default;
	NodeMeshMap getNodeMesh() { return m_nodeMeshMap; }
private:
	NodeMeshMap m_nodeMeshMap;
private:
	void loadNode(aiNode* node, const aiScene* scene);
	NodeMesh loadMesh(aiMesh* mesh, const aiScene* scene);
};

class MeshFilter {
public:
	~MeshFilter() = default;
	static MeshFilter* instance();
private:
	MeshFilter() = default;
};