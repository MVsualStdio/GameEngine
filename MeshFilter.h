#pragma once 

#include "D3D11Context.h"
#include "IDrawer.h"


class Geometry {
public:
	static VertexUVData CreateTrangle(Eigen::Vector3f points[3]);
	static VertexUVData CreateCube(float width, float height, float depth);
};