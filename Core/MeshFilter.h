#pragma once 

#include "D3D11Context.h"
#include "IDrawer.h"


class Geometry {
public:
	static std::shared_ptr<AnyVertexBuffer> CreateCube(float width, float height, float depth);
	static std::shared_ptr<AnyVertexBuffer> CreateTrangle(Eigen::Vector3f points[3]);
};