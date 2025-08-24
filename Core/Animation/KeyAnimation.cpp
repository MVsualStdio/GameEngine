#include "KeyAnimation.h"
#include <iostream>

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
    : m_name(name)
    , m_id(ID) 
    , m_transform(false) {

    m_numPosition = channel->mNumPositionKeys;
    for (int k = 0; k < m_numPosition; ++k) {
        aiVector3D aiPosition = channel->mPositionKeys[k].mValue;
        float timeStamp = channel->mPositionKeys[k].mTime;
        VectorKey key;
        key.timePos = timeStamp;
        key.value = { aiPosition.x, aiPosition.y ,aiPosition.z };
        m_positions.push_back(key);
    }

}

float Bone::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) {
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

int Bone::getPositionIndex(float animTime) {
    for (int i = 0; i < m_numPosition - 1; ++i) {
        if (animTime < m_positions[i + 1].timePos) {
            return i;
        }
    }
    return -1;
}

void Bone::interpolatePosition(float animationTime) {
    if (m_numPosition == 1) {
        return;
    }
    int p0Index = getPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float factor = getScaleFactor(m_positions[p0Index].timePos,
        m_positions[p1Index].timePos, animationTime);
    Eigen::Vector3f position = factor * m_positions[p0Index].value + (1 - factor) * m_positions[p1Index].value;
    m_transform.setPosition(position);
}

void Bone::update(float animationTime) {
    interpolatePosition(animationTime);
}

Transform Bone::getTransform() {
    return m_transform;
}

KeyAnimation::KeyAnimation(std::string path, Model* model){
	const aiScene* scene = LoadScene::importer()->ReadFile(path, LoadScene::loadFlag());
    m_animation = scene->mAnimations[0];
    m_duration = m_animation->mDuration;
    m_ticksPerSecond = m_animation->mTicksPerSecond != 0 ? m_animation->mTicksPerSecond : 25.0f;
    readHierarchyData(m_RootNode, scene->mRootNode);
    readMissingBones(m_animation, *model);
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

void KeyAnimation::readHierarchyData(AssimpNodeData& dest, const aiNode* src) {
    dest.name = src->mName.data;
    dest.transformation = ConvertMatrixToEigenFormat(src->mTransformation);
    dest.childrenCount = src->mNumChildren;
    for (int i = 0; i < src->mNumChildren; ++i) {
        AssimpNodeData newData;
        readHierarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

void KeyAnimation::readMissingBones(const aiAnimation* animation, Model& model) {
    int size = animation->mNumChannels;
    std::unordered_map<std::string, BoneInfo> bones = model.bone;
    int& boneCount = model.boneCount;
    for (int i = 0; i < size; i++) {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (bones.find(boneName) == bones.end()) {
            bones[boneName].id = boneCount;
            boneCount++;
        }
        m_bone[boneName] = Bone(channel->mNodeName.data,
            bones[channel->mNodeName.data].id, channel);
    }
    m_boneInfo = bones;
}

void KeyAnimation::calculateBoneTransform(float time, const AssimpNodeData* node, Eigen::Matrix4f parentTransform) {
    std::string nodeName = node->name;
    Eigen::Matrix4f finalTransform = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f globalTransformation = parentTransform;
    if (m_bone.find(nodeName) != m_bone.end() && m_boneInfo.find(nodeName) != m_boneInfo.end()) {

        Bone bone = m_bone[nodeName];
        bone.update(time);
        Eigen::Matrix4f nodeTransform = bone.getTransform().getMatrix();
        globalTransformation = parentTransform * nodeTransform;
        finalTransform = m_boneInfo[nodeName].offset * globalTransformation;
    }

    m_boneTransform[m_boneInfo[nodeName].id] = finalTransform;
    for (int i = 0; i < node->childrenCount; i++) {
        calculateBoneTransform(time, &node->children[i], globalTransformation);
    }
}

void KeyAnimation::update(float animationTime) {
    float time = std::fmod(animationTime, m_duration);
    calculateBoneTransform(time, &m_RootNode, Eigen::Matrix4f::Identity());
}