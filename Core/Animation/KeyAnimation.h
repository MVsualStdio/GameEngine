#pragma once

#include "../LoadScence.h"
#include "../MeshFilter.h"
#include <unordered_map>
#include <Eigen/Core>
#include "../Component/Transform.h"
struct AssimpNodeData
{
	Eigen::Matrix4f transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

struct VectorKey {
	float timePos;
	Eigen::Vector3f value;
};

class Bone
{
public:
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);
	Bone() = default;
	void update(float animationTime);
	Transform getTransform();
private:
	int getPositionIndex(float animationTime);
	void interpolatePosition(float animationTime);
	float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
private:
	std::vector<VectorKey> m_positions;
	std::string m_name;
	int m_id;
	Transform m_transform;
	unsigned int m_numPosition;
};

class KeyAnimation {
public:
	KeyAnimation(std::string path, Model* model); 
	
	void update(float animationTime);
	std::unordered_map<int, Eigen::Matrix4f> getfinalVertex() { return m_boneTransform; }

private:
	std::unordered_map<std::string, std::vector<VectorKey>> m_animPosition;
	std::unordered_map<std::string, Bone> m_bone;
	std::unordered_map<std::string, BoneInfo> m_boneInfo;
	std::unordered_map<int, Eigen::Matrix4f> m_boneTransform;

	aiAnimation* m_animation;
	double m_duration;
	double m_ticksPerSecond;
	AssimpNodeData m_RootNode;

	double m_CurrentTime = 0.0;
	
private:
	void readHierarchyData(AssimpNodeData& dest, const aiNode* src);
	void readMissingBones(const aiAnimation* animation, Model& model);
	void calculateBoneTransform(float time, const AssimpNodeData* node, Eigen::Matrix4f parentTransform);
};


//return true;