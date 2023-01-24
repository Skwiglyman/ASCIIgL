#pragma once

#include <array>

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

typedef struct Vertex
{
private:

public:
	void refactorPtrs()
	{
		x = &data.at(0);
		y = &data.at(1);
		z = &data.at(2);
		w = &data.at(3);
		u = &data.at(4);
		v = &data.at(5);
		uvw = &data.at(6);
		nx = &data.at(7);
		ny = &data.at(8);
		nz = &data.at(9);
	}

	std::array<float, 10> data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	float* x = &data.at(0);
	float* y = &data.at(1);
	float* z = &data.at(2);
	float* w = &data.at(3);
	float* u = &data.at(4);
	float* v = &data.at(5);
	float* uvw = &data.at(6);
	float* nx = &data.at(7);
	float* ny = &data.at(8);
	float* nz = &data.at(9);

	Vertex(std::array<float, 10> indata)
		: data(indata)
	{
		refactorPtrs();
	}

	Vertex() {}

	Vertex(const Vertex& t)
		: data(t.data)
	{

		refactorPtrs();
	}

	glm::vec3 GetXYZ() { return glm::vec3(*x, *y, *z); }
	glm::vec4 GetXYZW() { return glm::vec4(*x, *y, *z, *w); }
	glm::vec2 GetUVW() { return glm::vec3(*u, *v, *uvw); }
	glm::vec3 GetNXYZ() { return glm::vec3(*nx, *ny, *nz); }

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

	void SetUV(glm::vec2 inUV)
	{
		*u = inUV.x;
		*v = inUV.y;
	}

	void SetUVW(glm::vec3 inUVW)
	{
		*u = inUVW.x;
		*v = inUVW.y;
		*uvw = inUVW.z;
	}

	void SetNXYZ(glm::vec3 inNXYZ)
	{
		*nx = inNXYZ.x;
		*ny = inNXYZ.y;
		*nz = inNXYZ.z;
	}

} VERTEX;