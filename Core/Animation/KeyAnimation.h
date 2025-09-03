#pragma once
#include "../MeshFilter.h"
#include "../Component/Transform.h"

class KeyAnimationClip {
public:
	struct VectorKey {
		float timePos;
		Eigen::Vector3f value;
	};
	struct RotKey {
		float timePos;
		Eigen::Quaternionf value;
	};

	KeyAnimationClip();
	void init(const aiNodeAnim* channel);
	Transform update(float animationTime);
private:
	int getPositionIndex(float animationTime);
	void interpolatePosition(float animationTime);

	int getScaleIndex(float animationTime);
	void interpolateScale(float animationTime);

	int getRotationIndex(float animationTime);
	void interpolateRotation(float animationTime);

	float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
private:
	Transform m_transform;

	std::vector<RotKey> m_rotation;
	std::vector<VectorKey> m_positions;
	std::vector<VectorKey> m_scale;

};

class MeshFrameAnimationClip {
public:
	struct MorphKey {
		size_t frameIndex = 0;
		double time = 0.0;
		std::vector<unsigned int> targetIndices;
		std::vector<float> weights;
	};

	MeshFrameAnimationClip() = default;
	void init(const aiMeshMorphAnim* channel);
	MorphKey update(float animationTime);
	MorphKey getKey() { return m_keys[m_currentKey]; }
private:
	std::vector<MorphKey> m_keys ;
	int m_currentKey = 0;
};


struct NodeAnim{
	std::string name;
	Eigen::Matrix4f transformation;
	std::vector<NodeAnim> children;
	KeyAnimationClip animClip;
	MeshFrameAnimationClip frameClip;
	bool activate = false;
};



class LoadKeyAnimation {
public:
	LoadKeyAnimation(std::string path);
	std::unordered_map<std::string, NodeAnim> getNodeAnim();
	bool isSucessful() { return m_animNumber > 0; };
	float getDuration() { return m_duration; }
	NodeAnim* getRootNode() { return &m_rootNode; }
private:
	std::unordered_map<std::string, NodeAnim> m_nodeAnimMap;

	NodeAnim m_rootNode;
	aiAnimation* m_animation;
	float  m_duration;
	float m_ticksPerSecond;
	int m_animNumber;
	void readAnimNodes(NodeAnim& dest, aiNode* src);
	void readAnimChannel(const aiAnimation* animation);
	
};

class Animation : public Component{
public:
	Animation();
	bool init(std::string path);
	void update(double dt) override;
	std::unordered_map<std::string, Eigen::Matrix4f> getAnimMat() { return m_nodeAnimUpdateMap; }
	std::vector<Eigen::Matrix4f> getBoneAnimMat(NodeMesh& mesh, Eigen::Matrix4f& world);
	MeshFrameAnimationClip::MorphKey getFrameAnim(NodeMesh& mesh);
	void updateHelper(std::unordered_map<std::string, MeshRender*>& renders, std::unordered_map<std::string, NodeMesh> nodeMeshs);

private:
	std::shared_ptr<LoadKeyAnimation> m_load;
	std::unordered_map<std::string, NodeAnim> m_nodeAnimMap;
	double m_CurrentTime = 0.0;
	std::unordered_map<std::string, Eigen::Matrix4f> m_nodeAnimUpdateMap;
	void calculateTransform(float time, const NodeAnim* node, Eigen::Matrix4f parentTransform);
};