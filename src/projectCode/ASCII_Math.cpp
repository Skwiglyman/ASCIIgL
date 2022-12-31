#include "ASCII_Math.hpp"

#include <iostream>
#include "Vertex.hpp"

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

VERTEX lineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd)
{
	glm::normalize(planeN);
	float planeD = -glm::dot(planeN, planeP);
	float ad = glm::dot(lineStart, planeN);
	float bd = glm::dot(lineEnd, planeN);

	float t = (-planeD - ad) / (bd - ad);
	
	glm::vec3 line = lineEnd - lineStart;
	glm::vec3 lineIntersect = line * t;

	VERTEX newVert;
	newVert.SetXYZW(glm::vec4(lineStart + lineIntersect, 1.0f));

	return newVert;
}

VERTEX homogenousPlaneIntersect(VERTEX v2, VERTEX v1, int component, bool Near)
{

	// v2 is the vertex that is actually visible, v1 is behind the near plane

	VERTEX newVert = v1;
	VERTEX v = v1;
	
	v.SetXYZW(v1.GetXYZW() - v2.GetXYZW());

	float i0 = v1.data.at(component);
	float w0 = *v1.w;

	float vi = v.data.at(component);
	float vw = *v.w;

	float t;

	if (Near == true)
	{
		t = (i0 + w0) / (vw + vi); // works with near clipping
	}

	else
	{
		t = (i0 - w0) / (vi - vw); // works with far clipping
	}


	for (int i = 0; i < v1.data.size(); i++)
	{
		newVert.data.at(i) = glm::mix(v1.data.at(i), v2.data.at(i), t);
	}

	return newVert;
}