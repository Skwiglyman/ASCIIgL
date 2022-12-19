#include "ASCII_Math.hpp"

glm::vec3 calcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, bool out)
{
	glm::vec3 normal;

	glm::vec3 U = p2 - p1;
	glm::vec3 V = p3 - p1;

	if (out == true)
		normal = glm::cross(U, V);
	else
		normal = glm::cross(V, U);
	return glm::normalize(normal);
}

void vec4ToVert(glm::vec4 p, std::vector<float>* vert)
{
	vert->push_back(p.x);
	vert->push_back(p.y);
	vert->push_back(p.z);
	vert->push_back(p.w);
}

void vec3ToVert(glm::vec3 p, std::vector<float>* vert)
{
	vert->push_back(p.x);
	vert->push_back(p.y);
	vert->push_back(p.z);
}

void vec2ToVert(glm::vec2 p, std::vector<float>* vert)
{
	vert->push_back(p.x);
	vert->push_back(p.y);
}

glm::vec3 lineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd)
{
	glm::normalize(planeN);
	float planeD = -glm::dot(planeN, planeP);
	float ad = glm::dot(lineStart, planeN);
	float bd = glm::dot(lineEnd, planeN);

	float t = (-planeD - ad) / (bd - ad);
	
	glm::vec3 line = lineEnd - lineStart;
	glm::vec3 lineIntersect = line * t;
	return lineStart + lineIntersect;
}

glm::vec3 getPos(std::vector<float> vert)
{
	return glm::vec3(vert[0], vert[1], vert[2]);
}