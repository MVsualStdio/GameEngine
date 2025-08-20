#pragma once 

#include "D3D11Context.h"
#include "IDrawer.h"
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

class MeshFilter {
public:

	struct MeshInfo {
		std::shared_ptr<AnyVertexBuffer> vertexs;
		std::shared_ptr<Texture2D> textures;
	};

	~MeshFilter() = default;
	std::vector<MeshInfo> getMeshs(std::string path);
	static MeshFilter* instance();
private:
	static std::unordered_map<std::string, std::vector<MeshFilter::MeshInfo>> gMeshs;
	MeshFilter();

	bool loadScene(std::string path);
	bool processNode(std::string& key, aiNode* node, const aiScene* scene);
	MeshInfo processMesh(aiMesh* mesh, const aiScene* scene);
};