#include "ASCIIgLEngine.h"

glm::vec3 CalcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, bool out)
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

VERTEX LineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd)
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

VERTEX HomogenousPlaneIntersect(VERTEX v2, VERTEX v1, int component, bool Near)
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


	for (int i = 0; i < 6; i++)
	{
		newVert.data.at(i) = glm::mix(v1.data.at(i), v2.data.at(i), t);
	}

	return newVert;
}


void Clipping(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped, int component, bool Near, int i)
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

void ClippingHelper(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped, int i)
{
	std::vector<VERTEX> c1, c2, c3, c4, c5;

	Clipping(vertices, c1, 2, true, i);
	for (int k = 0; k < c1.size(); k += 3)
		Clipping(c1, c2, 2, false, k);
	for (int k = 0; k < c2.size(); k += 3)
		Clipping(c2, c3, 1, true, k);
	for (int k = 0; k < c3.size(); k += 3)
		Clipping(c3, c4, 1, false, k);
	for (int k = 0; k < c4.size(); k += 3)
		Clipping(c4, c5, 0, true, k);
	for (int k = 0; k < c5.size(); k += 3)
		Clipping(c5, clipped, 0, false, k);
}

bool BackFaceCull(VERTEX v1, VERTEX v2, VERTEX v3, bool CCW) // function that returns a negative if face is not culled
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

void PerspectiveDivision(std::vector<VERTEX>& clipCoords, int i)
{
	*clipCoords[i].x /= *clipCoords[i].w;
	*clipCoords[i].y /= *clipCoords[i].w;
	*clipCoords[i].z /= *clipCoords[i].w;
	*clipCoords[i].w = *clipCoords[i].w;
}

std::vector<glm::vec2> DrawLineBrensenham(int x1, int b, int x2, int y2) // I ripped this straight off github idk how it works
{
	std::vector<glm::vec2> points;

	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x, y;

	dx = x2 - x1;
	dy = y2 - b;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = 1;
	if (x2 < x1)
		incx = -1;
	incy = 1;
	if (y2 < b)
		incy = -1;
	x = x1;
	y = b;
	if (dx > dy)
	{
		points.push_back(glm::vec2(x, y));
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		for (i = 0; i < dx; i++)
		{
			if (e >= 0)
			{
				y += incy;
				points.push_back(glm::vec2(x, y));
				e += inc1;
				x += incx;
			}
			else
			{
				e += inc2; x += incx;
				//points.push_back(glm::vec2(x, y - incy));
			}
			points.push_back(glm::vec2(x, y));
		}
	}
	else
	{
		points.push_back(glm::vec2(x, y));
		e = 2 * dx - dy;
		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;
		for (i = 0; i < dy; i++)
		{
			if (e >= 0)
			{
				x += incx;
				points.push_back(glm::vec2(x, y));
				y += incy;
				e += inc1;

			}
			else
			{
				e += inc2; y += incy;
				//points.push_back(glm::vec2(x + incx, y));
			}

			points.push_back(glm::vec2(x, y));
		}
	}

	return points;
}

std::vector<glm::vec2> FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	std::vector<glm::vec2> pixels;

	auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
	auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) pixels.push_back(glm::vec2(i, ny)); };

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
	if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
	if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // swap values
		SWAP(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);    // Draw line from min to max points found on the y
									 // Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		SWAP(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3) return pixels;
	}
	return pixels;
}

