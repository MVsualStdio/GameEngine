#pragma once 

#include "D3D11Context.h"
#include "IDrawer.h"
#include <string>
#include "LoadScence.h"
#include "VertexLayout.h"

class Geometry {
public:
	static std::shared_ptr<AnyVertexBuffer> CreateCube(float width, float height, float depth);
	static std::shared_ptr<AnyVertexBuffer> CreateTrangle(Eigen::Vector3f points[3]);
	static std::shared_ptr<AnyVertexBuffer> CreatePlane(float width, float depth, float texU, float texV);
	static std::shared_ptr<AnyVertexBuffer> CreateCube(float width, float height, float depth, Eigen::Vector4f color);
};

struct BoneInfo
{
	/*id is index in finalBoneMatrices*/
	int id;

	/*offset matrix transforms vertex from model space to bone space*/
	Eigen::Matrix4f offset;

};

struct MeshInfo {
	std::shared_ptr<AnyVertexBuffer> vertexs;
	std::shared_ptr<Texture2D> textures;
};

struct Model {
	std::vector<MeshInfo> mesh;
	std::unordered_map<std::string, BoneInfo> bone;
	int boneCount;
};

class MeshFilter {
public:

	~MeshFilter() = default;
	std::vector<MeshInfo> getMeshInfo(std::string path);
	Model getModel(std::string path);
	static MeshFilter* instance();
private:
	static std::unordered_map<std::string, Model> gMeshs;
	
	MeshFilter() = default;

	bool processNode(std::string& key, aiNode* node, const aiScene* scene);
	void processMesh(std::string& key, aiMesh* mesh, const aiScene* scene);

	void extractBoneWeightForVertices(std::string& key, std::shared_ptr<AnyVertexBuffer> buffer, aiMesh* mesh, const aiScene* scene);
};