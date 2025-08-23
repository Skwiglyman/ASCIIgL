#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

namespace CollisionUtil {
	bool PointLineCol2D(glm::vec2 p, glm::vec2 lineStart, glm::vec2 lineEnd); // gets if a point is colliding with a line
	glm::vec3 LineCircleCol2D(glm::vec2 c, float r, glm::vec2 lineStart, glm::vec2 lineEnd); // gets if a line is colliding with a circle
	bool PointCircleCol2D(glm::vec2 p, glm::vec2 c, float r); // gets if a point is colliding with a circle
	glm::vec3 LineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd); // gets where the line meets the plane
};