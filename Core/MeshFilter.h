#pragma once 

#include "D3D11Context.h"
#include "IDrawer.h"
#include "VertexLayout.h"
#include <string>
#include "LoadScence.h"

class Geometry {
public:
	static std::shared_ptr<AnyVertexBuffer> CreateCube(float width, float height, float depth);
	static std::shared_ptr<AnyVertexBuffer> CreateTrangle(Eigen::Vector3f points[3]);
	static std::shared_ptr<AnyVertexBuffer> CreatePlane(float width, float depth, float texU, float texV);
	static std::shared_ptr<AnyVertexBuffer> CreateCube(float width, float height, float depth, Eigen::Vector4f color);
};

const int NUM_BONES_PER_VERTEX = 1;

struct NodeMesh {

	struct BoneMesh {
		float weights[NUM_BONES_PER_VERTEX];
		std::string name[NUM_BONES_PER_VERTEX];
		int indexs[NUM_BONES_PER_VERTEX] = { -1 };

		void addBone(int index, std::string nodeName, float weight) {
			for (size_t i = 0; i < NUM_BONES_PER_VERTEX; i++) {
				if (weights[i] <= 0.0f) {
					name[i] = nodeName;
					weights[i] = weight;
					indexs[i] = index;
					return;
				}
			}
		}
	};

	struct TextureMesh {
		enum class TextureType {
			diffuse,
			specular,
			normal,
			height
		};
		TextureType type;
		std::string path;
	};

	std::shared_ptr<VertexBuffer<VertexAnimation>> vertexs;
	std::vector<TextureMesh> textures;
	std::vector<BoneMesh> bones;

	std::unordered_map<std::string, std::pair<int,Eigen::Matrix4f>> boneIndexMap;
	bool hasBone = false;
};

class LoadMesh {
public:
	using NodeMeshMap = std::unordered_map<std::string, NodeMesh>;
	LoadMesh(std::string path);
	~LoadMesh() = default;
	NodeMeshMap getNodeMesh() { return m_nodeMeshMap; }
private:
	NodeMeshMap m_nodeMeshMap;
	std::string m_rootPath;

private:
	void loadNode(aiNode* node, const aiScene* scene);
	NodeMesh loadMesh(aiMesh* mesh, const aiScene* scene);
	int loadBone(NodeMesh& pVertex, aiMesh* mesh, const aiScene* scene);
	void loadMaterialTextures(std::vector<NodeMesh::TextureMesh>& textures, aiMaterial* mat, aiTextureType type, NodeMesh::TextureMesh::TextureType textype);
};

class MeshFilter {
public:
	~MeshFilter() = default;
	static MeshFilter* instance();
private:
	MeshFilter() = default;
};