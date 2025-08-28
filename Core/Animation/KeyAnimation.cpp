#include "KeyAnimation.h"
#include "../LoadScence.h"
#include <iostream>

KeyAnimationClip::KeyAnimationClip()
    : m_transform(false) {

}

void KeyAnimationClip::init(const aiNodeAnim* channel) {
    for (int k = 0; k < channel->mNumPositionKeys; ++k) {
        aiVector3D aiPosition = channel->mPositionKeys[k].mValue;
        float timeStamp = channel->mPositionKeys[k].mTime;
        VectorKey key;
        key.timePos = timeStamp;
        key.value = { aiPosition.x, aiPosition.y ,aiPosition.z };
        m_positions.push_back(key);
    }

    for (int k = 0; k < channel->mNumScalingKeys; ++k) {
        aiVector3D aiPosition = channel->mScalingKeys[k].mValue;
        float timeStamp = channel->mScalingKeys[k].mTime;
        VectorKey key;
        key.timePos = timeStamp;
        key.value = { aiPosition.x, aiPosition.y ,aiPosition.z };
        m_scale.push_back(key);
    }

    for (int k = 0; k < channel->mNumRotationKeys; ++k)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[k].mValue;
        float timeStamp = channel->mRotationKeys[k].mTime;
        RotKey data;
        data.value = { aiOrientation.w, aiOrientation.x,aiOrientation.y,aiOrientation.z };
        data.timePos = timeStamp;
        m_rotation.push_back(data);
    }
}

Transform KeyAnimationClip::update(float animationTime) {
    interpolatePosition(animationTime);
    interpolateRotation(animationTime);
    interpolateScale(animationTime);
    return m_transform;
}

float KeyAnimationClip::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) {
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

int KeyAnimationClip::getPositionIndex(float animTime) {
    int i = 0;
    for (; i < m_positions.size() - 1; ++i) {
        if (animTime < m_positions[i + 1].timePos) {
            return i;
        }
    }
    return i - 1;
}

int KeyAnimationClip::getScaleIndex(float animationTime) {
    int i = 0;
    for (; i < m_scale.size() - 1; ++i) {
        if (animationTime < m_scale[i + 1].timePos) {
            return i;
        }
    }
    return i - 1;
}

int KeyAnimationClip::getRotationIndex(float animationTime) {
    int i = 0;
    for (; i < m_rotation.size() - 1; ++i) {
        if (animationTime < m_rotation[i + 1].timePos) {
            return i;
        }
    }
    return i - 1;
}

void KeyAnimationClip::interpolateScale(float animationTime) {
    if (m_scale.size() <= 0) {
        return;
    }
    if (m_scale.size() == 1) {
        m_transform.setScale(m_scale[0].value);
        return;
    }
    int p0Index = getScaleIndex(animationTime);
    int p1Index = p0Index + 1;
    float factor = getScaleFactor(m_scale[p0Index].timePos,
        m_scale[p1Index].timePos, animationTime);
    Eigen::Vector3f scale = factor * m_scale[p0Index].value + (1 - factor) * m_scale[p1Index].value;
    m_transform.setScale(scale);
}

void KeyAnimationClip::interpolateRotation(float animationTime) {
    if (m_rotation.size() <= 0) {
        return;
    }
    if (m_rotation.size() == 1) {
        m_transform.setQuaternion(m_rotation[0].value);
        return;
    }
    int p0Index = getRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    float factor = getScaleFactor(m_rotation[p0Index].timePos,
        m_rotation[p1Index].timePos, animationTime);
    Eigen::Quaternionf rot = m_rotation[p0Index].value.slerp((1 - factor), m_rotation[p1Index].value);
    m_transform.setQuaternion(rot);
}

void KeyAnimationClip::interpolatePosition(float animationTime) {
    if (m_positions.size() <= 0) {
        return;
    }
    if (m_positions.size() == 1) {
        m_transform.setPosition(m_positions[0].value);
        return;
    }
    int p0Index = getPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float factor = getScaleFactor(m_positions[p0Index].timePos,
        m_positions[p1Index].timePos, animationTime);
    Eigen::Vector3f position = factor * m_positions[p0Index].value + (1 - factor) * m_positions[p1Index].value;
    m_transform.setPosition(position);
}

LoadKeyAnimation::LoadKeyAnimation(std::string path) {
    const aiScene* scene = LoadScene::importer()->ReadFile(path, LoadScene::loadFlag());
    int num = scene->mNumAnimations;
    m_animation = scene->mAnimations[0];
    m_duration = m_animation->mDuration;
    m_ticksPerSecond = m_animation->mTicksPerSecond != 0 ? m_animation->mTicksPerSecond : 25.0f;
    readAnimNodes(m_rootNode, scene->mRootNode);
    readAnimChannel(m_animation);
}

std::unordered_map<std::string, NodeAnim> LoadKeyAnimation::getNodeAnim() {
    return m_nodeAnimMap;
}




void LoadKeyAnimation::readAnimNodes(NodeAnim& dest, aiNode* src) {
    dest.name = src->mName.data;
    
    if (m_nodeAnimMap.find(dest.name) != m_nodeAnimMap.end()) {
        std::string baseName = dest.name + std::string("_$Assimp");
        for (int i = 0; m_nodeAnimMap.find(dest.name) != m_nodeAnimMap.end(); ++i) {
            dest.name = baseName + std::to_string(i);
        }
    }

    dest.transformation = Assimp2Eigen::ConvertMatrixToEigenFormat(src->mTransformation);
    for (int i = 0; i < src->mNumChildren; ++i) {
        NodeAnim node;
        readAnimNodes(node, src->mChildren[i]);
        dest.children.push_back(node);
    }

    m_nodeAnimMap[dest.name] = dest;
}

void LoadKeyAnimation::readAnimChannel(const aiAnimation* animation) {
    for (int i = 0; i < animation->mNumChannels; i++) {
        const aiNodeAnim* channel = animation->mChannels[i];
        
        std::string channelName = channel->mNodeName.data;
        std::cout << channelName << std::endl;

        if (m_nodeAnimMap.find(channelName) != m_nodeAnimMap.end()) {
            m_nodeAnimMap[channelName].activate = true;
            m_nodeAnimMap[channelName].animClip.init(channel);
        }

    }
}


Animation::Animation(std::string path)
    : m_load(path) {
    m_nodeAnimMap = m_load.getNodeAnim();
}

std::vector<Eigen::Matrix4f> Animation::getBoneAnimMat(NodeMesh& mesh, Eigen::Matrix4f& world) {
    if (mesh.hasBone) {
        std::vector<Eigen::Matrix4f> res(mesh.boneIndexMap.size());
        for (auto item : mesh.boneIndexMap) {
            std::string name = item.first;
            int index = item.second.first;
            Eigen::Matrix4f boneMat = item.second.second;
            res[index] = boneMat * m_nodeAnimUpdateMap[name] * world.inverse();
        }
        return res;
    }
    return std::vector<Eigen::Matrix4f>();
}

void Animation::update(double dt) {
    m_CurrentTime += dt;
    m_CurrentTime = std::fmod(m_CurrentTime, m_load.getDuration());
    calculateTransform(m_CurrentTime, m_load.getRootNode(), Eigen::Matrix4f::Identity());
}

void Animation::calculateTransform(float time, const NodeAnim* node, Eigen::Matrix4f parentTransform) {
    std::string nodeName = node->name;
    NodeAnim animNode = m_nodeAnimMap[nodeName];

    Eigen::Matrix4f nodeTransform = node->transformation;

    if (m_nodeAnimMap.find(nodeName) != m_nodeAnimMap.end() && animNode.activate) {
        Transform transform = animNode.animClip.update(time);
        nodeTransform = transform.getMatrix();
    }

    Eigen::Matrix4f globalTransformation = nodeTransform * parentTransform;
    m_nodeAnimUpdateMap[nodeName] = globalTransformation;

    for (int i = 0; i < animNode.children.size(); i++) {
        calculateTransform(time, &node->children[i], globalTransformation);
    }
}