#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Eigen/Core>

namespace Assimp2Eigen {
	Eigen::Matrix4f ConvertMatrixToEigenFormat(const aiMatrix4x4& from);
}


class LoadScene {
public:
	static unsigned int loadFlag();
	static Assimp::Importer* importer();
};