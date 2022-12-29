#include "ASCII_Math.hpp"

#include <iostream>

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

std::vector<float> lineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd)
{
	glm::normalize(planeN);
	float planeD = -glm::dot(planeN, planeP);
	float ad = glm::dot(lineStart, planeN);
	float bd = glm::dot(lineEnd, planeN);

	float t = (-planeD - ad) / (bd - ad);
	
	glm::vec3 line = lineEnd - lineStart;
	glm::vec3 lineIntersect = line * t;

	std::vector<float> newVert;
	vec4ToVert(glm::vec4(lineStart + lineIntersect, 1.0f), &newVert);

	return newVert;
}

std::vector<float> boundaryCoord(std::vector<float> v1, std::vector<float> v2)
{
	std::vector<float> newVert;
	float lerpAmt = 0;

	if (v1.size() == v2.size())
	{
		for (int i = 0; i < 3; i++)
		{
			lerpAmt = (v1[3] - v1[i]) / ((v1[3] - v1[i]) - (v2[3] - v2[i]));
			newVert.push_back(std::lerp(v2[i], v1[i], lerpAmt));
		}
	}
	newVert.push_back(0.01);

	return newVert;
}

glm::vec3 getPos(std::vector<float> vert)
{
	return glm::vec3(vert[0], vert[1], vert[2]);
}