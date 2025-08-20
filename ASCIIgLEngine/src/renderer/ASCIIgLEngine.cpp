#include "ASCIIgLEngine.hpp"

#include <algorithm>

glm::vec3 ASCIIgLEngine::CalcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, bool out)
{
	// this just returns the normal of a triangle but returning the cross product of the plane
	// which will be the normal either facing in or out
	glm::vec3 normal;

	glm::vec3 U = p2 - p1;
	glm::vec3 V = p3 - p1;

	if (out == true)
		normal = glm::cross(U, V);
	else
		normal = glm::cross(V, U);
	return glm::normalize(normal);
}

float ASCIIgLEngine::CalcDist2D(glm::vec2 p1, glm::vec2 p2)
{
	// simple function to calculate distance between 2 points using pythagorean theorum
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) * 1.0);
}

glm::mat4 ASCIIgLEngine::CalcModelMatrix(glm::vec3 position, glm::vec2 rotation, glm::vec3 size)
{
	// this function just saves lines as this needs to be calculated alot
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));
	model = glm::scale(model, size);

	return model;
}

bool ASCIIgLEngine::pointLineCol2D(glm::vec2 p, glm::vec2 lineStart, glm::vec2 lineEnd)
{

	// get distance from the point to the two ends of the line
	float d1 = CalcDist2D(p, lineStart);
	float d2 = CalcDist2D(p, lineEnd);

	// get the length of the line
	float lineLen = CalcDist2D(lineStart, lineEnd);

	// since floats are so minutely accurate, add
	// a little buffer zone that will give collision
	float buffer = 0.1;    // higher # = less accurate

	// if the two distances are equal to the line's 
	// length, the point is on the line!
	// note we use the buffer here to give a range, 
	// rather than one #
	if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer) {
		return true;
	}
	return false;
}

glm::vec3 ASCIIgLEngine::lineCircleCol2D(glm::vec2 c, float r, glm::vec2 lineStart, glm::vec2 lineEnd)
{
	// get length of the line
	float len = CalcDist2D(lineStart, lineEnd);

	// get dot product of the line and circle
	float dot = (((c.x - lineStart.x) * (lineEnd.x - lineStart.x)) + ((c.y - lineStart.y) * (lineEnd.y - lineStart.y))) / pow(len, 2);

	// find the closest point on the line
	glm::vec2 closest;
	closest.x = lineStart.x + (dot * (lineEnd.x - lineStart.x));
	closest.y = lineStart.y + (dot * (lineEnd.y - lineStart.y));


	// is this point actually on the line segment?
	// if so keep going, but if not, return false
	bool onSegment = pointLineCol2D(closest, lineStart, lineEnd);
	if (!onSegment) return glm::vec3(0, 0, 0);

	// get distance to closest point
	float distance = CalcDist2D(closest, c);

	// if point is in the circle, line colleds with circle
	if (distance <= r) {
		return glm::vec3(1, closest);
	}
	return glm::vec3(0, 0, 0);
}

bool ASCIIgLEngine::pointCircle2D(glm::vec2 p, glm::vec2 c, float r)
{
	// if distance between centres is less than r, point is in circle
	return CalcDist2D(p, c) < r;
}

glm::vec3 ASCIIgLEngine::LineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd)
{
	// this interpolates where the line meets the plane to get the new position on the line of where the line meets the plane
	glm::normalize(planeN);
	float planeD = -glm::dot(planeN, planeP);
	float ad = glm::dot(lineStart, planeN);
	float bd = glm::dot(lineEnd, planeN);

	float t = (-planeD - ad) / (bd - ad);

	glm::vec3 line = lineEnd - lineStart;
	glm::vec3 lineIntersect = line * t;

	return lineStart + lineIntersect;
}

VERTEX ASCIIgLEngine::HomogenousPlaneIntersect(VERTEX v2, VERTEX v1, int component, bool Near)
{
	// this function interpolates on the line between both vectors to get a new point on the line between them
	// v2 is the vertex that is actually visible, v1 is behind the near plane

	// copying over vertices to make tex coords copy over too
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


	for (int i = 0; i < 6; i++)
	{
		newVert.data.at(i) = glm::mix(v1.data.at(i), v2.data.at(i), t); // finding new value of component based on interpolation
	}

	return newVert;
}


void ASCIIgLEngine::Clipping(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped, int component, bool Near)
{ // this function is my clipping algorithm that seperates triangles that are on view boundary and discard triangles fully outside of view boundary
	for (int i = 0; i < vertices.size(); i+=3)
	{
		std::vector<int> inside; // I hold the indices in the vertice list in these vectors
		std::vector<int> outside;

		VERTEX temp[6]; // I have this array so I can keep the winding order of the vertices after using the indexes from inside and outside
		int newTri = 0; // how many new triangles there are

		if (Near == true) // it has different conditions for it being outside view box depending on which clipping plane it is
		{
			if (vertices[i].data.at(component) > -*vertices[i].w) { inside.push_back(i); }
			else { outside.push_back(i); }

			if (vertices[i + 1].data.at(component) > -*vertices[i + 1].w) { inside.push_back(i + 1); }
			else { outside.push_back(i + 1); }

			if (vertices[i + 2].data.at(component) > -*vertices[i + 2].w) { inside.push_back(i + 2); }
			else { outside.push_back(i + 2); }
		}

		else // determining if vertices are outside clipping plane
		{
			if (vertices[i].data.at(component) < *vertices[i].w) { inside.push_back(i); }
			else { outside.push_back(i); }

			if (vertices[i + 1].data.at(component) < *vertices[i + 1].w) { inside.push_back(i + 1); }
			else { outside.push_back(i + 1); }

			if (vertices[i + 2].data.at(component) < *vertices[i + 2].w) { inside.push_back(i + 2); }
			else { outside.push_back(i + 2); }
		}

		if (inside.size() == 3) // if tri is inside, keep all
		{
			clipped.push_back(vertices[inside[0]]);
			clipped.push_back(vertices[inside[1]]);
			clipped.push_back(vertices[inside[2]]);
			continue;
		}

		else if (inside.size() == 1) // if there is 2 vertices outside, get 2 new vertices
		{
			VERTEX newPos1 = HomogenousPlaneIntersect(vertices[inside[0]], vertices[outside[0]], component, Near);
			VERTEX newPos2 = HomogenousPlaneIntersect(vertices[inside[0]], vertices[outside[1]], component, Near);

			temp[outside[0] - i] = newPos1;
			temp[outside[1] - i] = newPos2;
			temp[inside[0] - i] = vertices[inside[0]];
			newTri = 3;
		}

		else if (inside.size() == 2) // if one vertice is outside, new triangles have to be made
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
		for (int k = 0; k < newTri; k++) // this keeps winding order
			clipped.push_back(temp[k]);
	}
}

void ASCIIgLEngine::ClippingHelper(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped)
{
	std::vector<VERTEX> c1, c2, c3, c4, c5;
	// calls clipping on every clip plane
	Clipping(vertices, c1, 2, true);
	Clipping(c1, c2, 2, false);
	Clipping(c2, c3, 1, true);
	Clipping(c3, c4, 1, false);
	Clipping(c4, c5, 0, true);
	Clipping(c5, clipped, 0, false);
}

CHAR_INFO ASCIIgLEngine::GetColGlyph(float GreyScale)
{
	// returns colour combination at the fastest possible speed using gray scale value
	static const CHAR_INFO vals[16]
	{
		CHAR_INFO{ PIXEL_QUARTER, FG_BLACK}, CHAR_INFO{ PIXEL_QUARTER, FG_DARK_GREY},
		CHAR_INFO{ PIXEL_QUARTER, FG_GREY}, CHAR_INFO{ PIXEL_QUARTER, FG_WHITE},

		CHAR_INFO{ PIXEL_HALF, FG_BLACK}, CHAR_INFO{ PIXEL_HALF, FG_DARK_GREY},
		CHAR_INFO{ PIXEL_HALF, FG_GREY}, CHAR_INFO{ PIXEL_HALF, FG_WHITE},

		CHAR_INFO{ PIXEL_THREEQUARTERS, FG_BLACK}, CHAR_INFO{ PIXEL_THREEQUARTERS, FG_DARK_GREY},
		CHAR_INFO{ PIXEL_THREEQUARTERS, FG_GREY}, CHAR_INFO{ PIXEL_THREEQUARTERS, FG_WHITE},

		CHAR_INFO{ PIXEL_SOLID, FG_BLACK}, CHAR_INFO{ PIXEL_SOLID, FG_DARK_GREY},
		CHAR_INFO{ PIXEL_SOLID, FG_GREY}, CHAR_INFO{ PIXEL_SOLID, FG_WHITE},
	};

	return vals[static_cast<long>(std::min(GreyScale * 15.99f, 16.0f))]; // has to be 15.99 or it overflows
}

float ASCIIgLEngine::GrayScaleRGB(glm::vec3 rgb)
{
	return (0.3 * rgb.x + 0.6 * rgb.y + 0.1 * rgb.z); // grayscales based on how much we see that wavelength of light instead of just averaging
}

bool ASCIIgLEngine::BackFaceCull(VERTEX v1, VERTEX v2, VERTEX v3, bool CCW) // function that returns a negative if face is not culled
{ // determines if the triangle is in the correct winding order or not

	// it calculates it based on glm cross because if the perpendicular z is less than 0, the triangle is pointing away
	glm::vec3 U = v2.GetXYZ() - v1.GetXYZ();
	glm::vec3 V = v3.GetXYZ() - v1.GetXYZ();

	float crossZ;

	if (CCW == true)
		crossZ = glm::cross(U, V).z;
	else
		crossZ = glm::cross(V, U).z;

	return crossZ > 0.0f;
}

void ASCIIgLEngine::PerspectiveDivision(std::vector<VERTEX>& clipCoords, int i)
{
	// just saves lines on perspective division (dividing everything by w)
	*clipCoords[i].x /= *clipCoords[i].w;
	*clipCoords[i].y /= *clipCoords[i].w;
	*clipCoords[i].z /= *clipCoords[i].w;
	*clipCoords[i].w = *clipCoords[i].w;

	clipCoords[i].SetUVW(glm::vec3(clipCoords[i].GetUVW().x / *clipCoords[i].w, clipCoords[i].GetUVW().y / *clipCoords[i].w, 1.0f / *clipCoords[i].w));
}



