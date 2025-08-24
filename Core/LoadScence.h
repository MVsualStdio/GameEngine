#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class LoadScene {
public:
	static unsigned int loadFlag();
	static Assimp::Importer* importer();
};