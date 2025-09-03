#include <rttr/registration>
#include "KeyAnimation.h"
#include "../LoadScence.h"
#include <iostream>
#include "../MeshRender.h"
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

void MeshFrameAnimationClip::init(const aiMeshMorphAnim* channel){
    for (unsigned int j = 0; j < channel->mNumKeys; j++) {
        aiMeshMorphKey& key = channel->mKeys[j];

        MorphKey morphKey;
        morphKey.time = key.mTime;

        for (unsigned int k = 0; k < key.mNumValuesAndWeights; k++) {
            morphKey.targetIndices.push_back(key.mValues[k]);
            morphKey.weights.push_back(key.mWeights[k]);
        }
        m_keys.push_back(morphKey);
    }
    std::sort(m_keys.begin(), m_keys.end(),
        [](const MorphKey& a, const MorphKey& b) {
            return a.time < b.time;
    });
}

MeshFrameAnimationClip::MorphKey MeshFrameAnimationClip::update(float animationTime) {
    size_t nextKeyIndex = 0;
    if (m_keys.size() <= 0) {
        return MorphKey{};
    }

    for (; nextKeyIndex < m_keys.size(); nextKeyIndex++) {
        if (m_keys[nextKeyIndex].time > animationTime) {
            break;
        }
    }
    if (nextKeyIndex == 0) {
        m_currentKey = 0;
    }
    else if (nextKeyIndex >= m_keys.size()) {
        m_currentKey = m_keys.size() - 1;
    }
    else {
        m_currentKey = nextKeyIndex - 1;
    }

    m_keys[m_currentKey].frameIndex = m_currentKey;
    return m_keys[m_currentKey];
}

LoadKeyAnimation::LoadKeyAnimation(std::string path) {
    const aiScene* scene = LoadScene::importer()->ReadFile(path, LoadScene::loadFlag());
    m_animNumber = scene->mNumAnimations;
    if (scene->mNumAnimations > 0) {
        m_animation = scene->mAnimations[0];
        m_duration = m_animation->mDuration;
        m_ticksPerSecond = m_animation->mTicksPerSecond != 0 ? m_animation->mTicksPerSecond : 25.0f;
        readAnimNodes(m_rootNode, scene->mRootNode);
        readAnimChannel(m_animation);
    }
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
    std::cout << "Number of mesh channels: " << m_animation->mNumMeshChannels << std::endl;
    std::cout << "Number of Morph channels: " << m_animation->mNumMorphMeshChannels << std::endl;
   
    for (int i = 0; i < animation->mNumChannels; i++) {
        const aiNodeAnim* channel = animation->mChannels[i];
        
        std::string channelName = channel->mNodeName.data;
        std::cout << channelName << std::endl;

        if (m_nodeAnimMap.find(channelName) != m_nodeAnimMap.end()) {
            m_nodeAnimMap[channelName].activate = true;
            m_nodeAnimMap[channelName].animClip.init(channel);
        }
    }

    for (int i = 0; i < animation->mNumMorphMeshChannels; i++) {
        const aiMeshMorphAnim* channel = animation->mMorphMeshChannels[i];

        std::string channelName = channel->mName.data;

        std::cout << channelName << std::endl;

        if (m_nodeAnimMap.find(channelName) != m_nodeAnimMap.end()) {
            m_nodeAnimMap[channelName].activate = true;
            m_nodeAnimMap[channelName].frameClip.init(channel);
        }
        
    }

}



RTTR_REGISTRATION
{
    rttr::registration::class_<Animation>("Animation")
            .constructor<>()(rttr::policy::ctor::as_raw_ptr);
}


Animation::Animation(){
    
}

bool Animation::init(std::string path) {
    m_load = std::make_shared<LoadKeyAnimation>(path);
    if (m_load->isSucessful()) {
        m_nodeAnimMap = m_load->getNodeAnim();
        update(0);
    }
    return m_load->isSucessful();
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

MeshFrameAnimationClip::MorphKey Animation::getFrameAnim(NodeMesh& mesh) {
    if (mesh.hasMorph) {
        MeshFrameAnimationClip::MorphKey key = m_nodeAnimMap[mesh.name].frameClip.getKey();
        return key;
    }       
    return  MeshFrameAnimationClip::MorphKey{};
}

void Animation::update(double dt) {
    m_CurrentTime += dt;
    m_CurrentTime = std::fmod(m_CurrentTime, m_load->getDuration());
    calculateTransform(m_CurrentTime, m_load->getRootNode(), Eigen::Matrix4f::Identity());
}

void Animation::calculateTransform(float time, const NodeAnim* node, Eigen::Matrix4f parentTransform) {
    std::string nodeName = node->name;
    NodeAnim& animNode = m_nodeAnimMap[nodeName];

    Eigen::Matrix4f nodeTransform = node->transformation;

    if (m_nodeAnimMap.find(nodeName) != m_nodeAnimMap.end() && animNode.activate) {
        Transform transform = animNode.animClip.update(time);
        nodeTransform = transform.getMatrix();

        MeshFrameAnimationClip::MorphKey key = animNode.frameClip.update(time);
    }

    Eigen::Matrix4f globalTransformation = nodeTransform * parentTransform;
    m_nodeAnimUpdateMap[nodeName] = globalTransformation;

    for (int i = 0; i < animNode.children.size(); i++) {
        calculateTransform(time, &node->children[i], globalTransformation);
    }
}


void Animation::updateHelper(std::unordered_map<std::string, MeshRender*>& renders, std::unordered_map<std::string, NodeMesh> nodeMeshs) {
    auto AnimMat = this->getAnimMat();

    for (auto render : renders) {
        Eigen::Matrix4f world = AnimMat[render.first];
        render.second->getMaterial()->getVSShader()->setUniform("g_World", world);
        int mi = 0;

        NodeMesh& mesh = nodeMeshs[render.first];

        if (mesh.hasBone) {
            std::vector<Eigen::Matrix4f> boneMat = this->getBoneAnimMat(mesh, world);
            render.second->getMaterial()->getVSShader()->setUniform("boneMatrices", boneMat);
        }

        if (mesh.hasMorph) {
            std::shared_ptr<VertexBuffer<VertexAnimation>> pVertexs = mesh.vertexs;
            MeshFrameAnimationClip::MorphKey key = this->getFrameAnim(mesh);
            for (int i = 0; i < key.targetIndices.size(); ++i) {
                float widget = key.weights[i];
                unsigned int targetIndex = key.targetIndices[i];
                NodeMesh::FrameMesh& targets = mesh.meshAnims[targetIndex];
                targets.weight = widget;
            }

            for (size_t j = 0; j < mesh.meshAnims.size(); j++) {
                NodeMesh::FrameMesh& targets = mesh.meshAnims[j];
                if (targets.weight > 0.0f) {
                    for (int k = 0; k < pVertexs->vertices.size(); ++k) {
                        pVertexs->vertices[k].pos += (targets.pos[k] - pVertexs->vertices[k].pos) * targets.weight;
                        if (targets.normal.size() > 0) {
                            pVertexs->vertices[k].normal += (targets.normal[k] - pVertexs->vertices[k].normal) * targets.weight;
                        }
                    }
                }
            }
            render.second->resetVertex();
            render.second->setVertex(std::make_shared<AnyVertexBuffer>(nodeMeshs[render.first].vertexs));
        }
    }
}