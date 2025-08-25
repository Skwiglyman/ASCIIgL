#include "CollisionUtil.hpp"

#include "../util/MathUtil.hpp"

bool CollisionUtil::PointLineCol2D(glm::vec2 p, glm::vec2 lineStart, glm::vec2 lineEnd)
{

	// get distance from the point to the two ends of the line
	float d1 = glm::distance(p, lineStart);
	float d2 = glm::distance(p, lineEnd);

	// get the length of the line
	float lineLen = glm::distance(lineStart, lineEnd);

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

glm::vec3 CollisionUtil::LineCircleCol2D(glm::vec2 c, float r, glm::vec2 lineStart, glm::vec2 lineEnd)
{
	// get length of the line
	float len = glm::distance(lineStart, lineEnd);

	// get dot product of the line and circle
	float dot = (((c.x - lineStart.x) * (lineEnd.x - lineStart.x)) + ((c.y - lineStart.y) * (lineEnd.y - lineStart.y))) / pow(len, 2);

	// find the closest point on the line
	glm::vec2 closest;
	closest.x = lineStart.x + (dot * (lineEnd.x - lineStart.x));
	closest.y = lineStart.y + (dot * (lineEnd.y - lineStart.y));


	// is this point actually on the line segment?
	// if so keep going, but if not, return false
	bool onSegment = PointLineCol2D(closest, lineStart, lineEnd);
	if (!onSegment) return glm::vec3(0, 0, 0);

	// get distance to closest point
	float distance = glm::distance(closest, c);

	// if point is in the circle, line colleds with circle
	if (distance <= r) {
		return glm::vec3(1, closest);
	}
	return glm::vec3(0, 0, 0);
}

bool CollisionUtil::PointCircleCol2D(glm::vec2 p, glm::vec2 c, float r)
{
	// if distance between centres is less than r, point is in circle
	return glm::distance(p, c) < r;
}

glm::vec3 CollisionUtil::LineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd)
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