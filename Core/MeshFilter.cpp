#include "MeshFilter.h"


VertexUVData Geometry::CreateTrangle(Eigen::Vector3f points[3]) {
	VertexUVData vertex;

	vertex.data.resize(3);
	vertex.data[0].pos = Eigen::Vector3<float>(points[0].x(), points[0].y(), points[0].z());
	vertex.data[1].pos = Eigen::Vector3<float>(points[1].x(), points[1].y(), points[1].z());
	vertex.data[2].pos = Eigen::Vector3<float>(points[2].x(), points[2].y(), points[2].z());

	vertex.data[0].color = Eigen::Vector4<float>(0.0f, 0.0f, 0.0f, 0.0f);
	vertex.data[1].color = Eigen::Vector4<float>(0.0f, 0.0f, 0.0f, 0.0f);
	vertex.data[2].color = Eigen::Vector4<float>(0.0f, 0.0f, 0.0f, 0.0f);

	vertex.data[0].uv = Eigen::Vector2<float>(1.0f, 0.0f);
	vertex.data[1].uv = Eigen::Vector2<float>(0.0f, 1.0f);
	vertex.data[2].uv = Eigen::Vector2<float>(1.0f, 1.0f);

	vertex.data[0].normal = Eigen::Vector3<float>(0.0f, 0.0f, 0.0f);
	vertex.data[1].normal = Eigen::Vector3<float>(0.0f, 0.0f, 0.0f);
	vertex.data[2].normal = Eigen::Vector3<float>(0.0f, 0.0f, 0.0f);

	vertex.index = { 0,1,2 };

	return vertex;
}

VertexUVData Geometry::CreateCube(float width, float height, float depth) {
	VertexUVData vertex;

	vertex.data.resize(24);
    float w2 = width / 2, h2 = height / 2, d2 = depth / 2;

    vertex.data[0].pos = Eigen::Vector3<float>(w2, -h2, -d2);
    vertex.data[1].pos = Eigen::Vector3<float>(w2, h2, -d2);
    vertex.data[2].pos = Eigen::Vector3<float>(w2, h2, d2);
    vertex.data[3].pos = Eigen::Vector3<float>(w2, -h2, d2);

    vertex.data[4].pos = Eigen::Vector3<float>(-w2, -h2, d2);
    vertex.data[5].pos = Eigen::Vector3<float>(-w2, h2, d2);
    vertex.data[6].pos = Eigen::Vector3<float>(-w2, h2, -d2);
    vertex.data[7].pos = Eigen::Vector3<float>(-w2, -h2, -d2);

    vertex.data[8].pos = Eigen::Vector3<float>(-w2, h2, -d2);
    vertex.data[9].pos = Eigen::Vector3<float>(-w2, h2, d2);
    vertex.data[10].pos = Eigen::Vector3<float>(w2, h2, d2);
    vertex.data[11].pos = Eigen::Vector3<float>(w2, h2, -d2);

    vertex.data[12].pos = Eigen::Vector3<float>(w2, -h2, -d2);
    vertex.data[13].pos = Eigen::Vector3<float>(w2, -h2, d2);
    vertex.data[14].pos = Eigen::Vector3<float>(-w2, -h2, d2);
    vertex.data[15].pos = Eigen::Vector3<float>(-w2, -h2, -d2);

    vertex.data[16].pos = Eigen::Vector3<float>(w2, -h2, d2);
    vertex.data[17].pos = Eigen::Vector3<float>(w2, h2, d2);
    vertex.data[18].pos = Eigen::Vector3<float>(-w2, h2, d2);
    vertex.data[19].pos = Eigen::Vector3<float>(-w2, -h2, d2);

    vertex.data[20].pos = Eigen::Vector3<float>(-w2, -h2, -d2);
    vertex.data[21].pos = Eigen::Vector3<float>(-w2, h2, -d2);
    vertex.data[22].pos = Eigen::Vector3<float>(w2, h2, -d2);
    vertex.data[23].pos = Eigen::Vector3<float>(w2, -h2, -d2);


    for (size_t i = 0; i < 4; ++i)
    {
        // 右面(+X面)
        vertex.data[i].normal = Eigen::Vector3<float>(1.0f, 0.0f, 0.0f);
        //geoData.tangents[i] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
        // 左面(-X面)
        vertex.data[i+4].normal = Eigen::Vector3<float>(-1.0f, 0.0f, 0.0f);
        //geoData.tangents[i + 4] = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
        // 顶面(+Y面)
        vertex.data[i+8].normal = Eigen::Vector3<float>(0.0f, 1.0f, 0.0f);
       // geoData.tangents[i + 8] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
        // 底面(-Y面)
        vertex.data[i+ 12].normal = Eigen::Vector3<float>(0.0f, -1.0f, 0.0f);
        //geoData.tangents[i + 12] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // 背面(+Z面)
        vertex.data[i + 16].normal = Eigen::Vector3<float>(0.0f, 0.0f, 1.0f);
        //geoData.tangents[i + 16] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // 正面(-Z面)
        vertex.data[i + 20].normal = Eigen::Vector3<float>(0.0f, 0.0f, -1.0f);
        //geoData.tangents[i + 20] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    for (size_t i = 0; i < 6; ++i)
    {
        vertex.data[i * 4].uv = Eigen::Vector2<float>(0.0f, 1.0f);
        vertex.data[i * 4 + 1].uv = Eigen::Vector2<float>(0.0f, 0.0f);
        vertex.data[i * 4 + 2].uv = Eigen::Vector2<float>(1.0f, 0.0f);
        vertex.data[i * 4 + 3].uv = Eigen::Vector2<float>(1.0f, 1.0f);
    }

    vertex.index = {
        0, 1, 2, 2, 3, 0,		// 右面(+X面)
        4, 5, 6, 6, 7, 4,		// 左面(-X面)
        8, 9, 10, 10, 11, 8,	// 顶面(+Y面)
        12, 13, 14, 14, 15, 12,	// 底面(-Y面)
        16, 17, 18, 18, 19, 16, // 背面(+Z面)
        20, 21, 22, 22, 23, 20	// 正面(-Z面)
    };
 
    return vertex;
}