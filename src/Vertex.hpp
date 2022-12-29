#pragma once

#include <array>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

typedef struct Vertex
{
public:
	std::array<float, 6> data;

	float* x = &data.at(0);
	float* y = &data.at(1);
	float* z = &data.at(2);
	float* w = &data.at(3);
	float* u = &data.at(4);
	float* v = &data.at(5);

	glm::vec3 GetXYZ() { return glm::vec3(*x, *y, *z); }
	glm::vec4 GetXYZW() { return glm::vec4(*x, *y, *z, *w); }
	glm::vec2 GetUV() { return glm::vec2(*u, *v); }

	void SetXYZ(glm::vec3 inXYZ)
	{
		*x = inXYZ.x;
		*y = inXYZ.y;
		*z = inXYZ.z;
	}

	void SetXYZW(glm::vec4 inXYZW)
	{
		*x = inXYZW.x;
		*y = inXYZW.y;
		*z = inXYZW.z;
		*w = inXYZW.w;
	}

	void SetUV(glm::vec4 inUV)
	{
		*u = inUV.x;
		*v = inUV.y;
	}

} VERTEX;