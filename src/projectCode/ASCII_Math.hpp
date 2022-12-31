#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include "Vertex.hpp"

glm::vec3 calcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, bool out); // calculates normals given 3 points

VERTEX lineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd);
VERTEX homogenousPlaneIntersect(VERTEX v1, VERTEX v2, int component, bool Near);