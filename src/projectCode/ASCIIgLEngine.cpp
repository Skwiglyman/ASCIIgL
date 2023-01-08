#include "ASCIIgLEngine.h"

glm::vec3 ASCIIgLEngine::CalcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, bool out)
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

VERTEX ASCIIgLEngine::LineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd)
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

VERTEX ASCIIgLEngine::HomogenousPlaneIntersect(VERTEX v2, VERTEX v1, int component, bool Near)
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


void ASCIIgLEngine::Clipping(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped, int component, bool Near, int i)
{
	std::vector<int> inside;
	std::vector<int> outside;

	VERTEX temp[6];
	int newTri = 0;

	if (Near == true)
	{
		if (vertices[i].data.at(component) > -*vertices[i].w) { inside.push_back(i); }
		else { outside.push_back(i); }

		if (vertices[i + 1].data.at(component) > -*vertices[i + 1].w) { inside.push_back(i + 1); }
		else { outside.push_back(i + 1); }

		if (vertices[i + 2].data.at(component) > -*vertices[i + 2].w) { inside.push_back(i + 2); }
		else { outside.push_back(i + 2); }
	}

	else
	{
		if (vertices[i].data.at(component) < *vertices[i].w) { inside.push_back(i); }
		else { outside.push_back(i); }

		if (vertices[i + 1].data.at(component) < *vertices[i + 1].w) { inside.push_back(i + 1); }
		else { outside.push_back(i + 1); }

		if (vertices[i + 2].data.at(component) < *vertices[i + 2].w) { inside.push_back(i + 2); }
		else { outside.push_back(i + 2); }
	}

	if (inside.size() == 3)
	{
		clipped.push_back(vertices[inside[0]]);
		clipped.push_back(vertices[inside[1]]);
		clipped.push_back(vertices[inside[2]]);
		return;
	}

	else if (inside.size() == 1)
	{
		VERTEX newPos1 = HomogenousPlaneIntersect(vertices[inside[0]], vertices[outside[0]], component, Near);
		VERTEX newPos2 = HomogenousPlaneIntersect(vertices[inside[0]], vertices[outside[1]], component, Near);

		temp[outside[0] - i] = newPos1;
		temp[outside[1] - i] = newPos2;
		temp[inside[0] - i] = vertices[inside[0]];
		newTri = 3;
	}

	else if (inside.size() == 2)
	{
		VERTEX newPos1 = HomogenousPlaneIntersect(vertices[inside[0]], vertices[outside[0]], component, Near);
		VERTEX newPos2 = HomogenousPlaneIntersect(vertices[inside[1]], vertices[outside[0]], component, Near);

		// triangle 1
		temp[inside[0] - i] = vertices[inside[0]];
		temp[inside[1] - i] = vertices[inside[1]];
		temp[outside[0] - i] = newPos1;

		// triangle 2
		temp[outside[0] - i + 3] = newPos2;
		temp[inside[0] - i + 3] = newPos1;
		temp[inside[1] - i + 3] = (vertices[inside[1]]);
		newTri = 6;
	}
	for (int k = 0; k < newTri; k++)
		clipped.push_back(temp[k]);
}

void ASCIIgLEngine::ClippingHelper(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped, int i)
{
	std::vector<VERTEX> c1, c2, c3, c4, c5;

	//std::cout << vertices.size() << std::endl;
	Clipping(vertices, c1, 2, true, i);

	//std::cout << c1.size() << std::endl;
	for (int k = 0; k < c1.size(); k += 3)
		Clipping(c1, c2, 2, false, k);

	//std::cout << c2.size() << std::endl;
	for (int k = 0; k < c2.size(); k += 3)
		Clipping(c2, c3, 1, true, k);

	//std::cout << c3.size() << std::endl;
	for (int k = 0; k < c3.size(); k += 3)
		Clipping(c3, c4, 1, false, k);

	//std::cout << c4.size() << std::endl;
	for (int k = 0; k < c4.size(); k += 3)
		Clipping(c4, c5, 0, true, k);

	//std::cout << c5.size() << std::endl;
	for (int k = 0; k < c5.size(); k += 3)
		Clipping(c5, clipped, 0, false, k);

	//std::cout << std::endl;
}

bool ASCIIgLEngine::BackFaceCull(VERTEX v1, VERTEX v2, VERTEX v3, bool CCW) // function that returns a negative if face is not culled
{
	glm::vec3 p1 = glm::vec3(*v1.x, *v1.y, *v1.z);
	glm::vec3 p2 = glm::vec3(*v2.x, *v2.y, *v2.z);
	glm::vec3 p3 = glm::vec3(*v3.x, *v3.y, *v3.z);

	glm::vec3 U = p2 - p1;
	glm::vec3 V = p3 - p1;

	float crossZ;

	if (CCW == true)
		crossZ = glm::cross(U, V).z;
	else
		crossZ = glm::cross(V, U).z;

	return crossZ > 0.0f;
}

void ASCIIgLEngine::PerspectiveDivision(std::vector<VERTEX>& clipCoords, int i)
{
	*clipCoords[i].x /= *clipCoords[i].w;
	*clipCoords[i].y /= *clipCoords[i].w;
	*clipCoords[i].z /= *clipCoords[i].w;
	*clipCoords[i].w = *clipCoords[i].w;
}



