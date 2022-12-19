#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

glm::vec3 calcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, bool out); // calculates normals given 3 points

// fast ways to add glm::vec to vector<float>
void vec4ToVert(glm::vec4 p, std::vector<float>* vert);
void vec3ToVert(glm::vec3 p, std::vector<float>* vert);
void vec2ToVert(glm::vec2 p, std::vector<float>* vert);

glm::vec3 getPos(std::vector<float> vert);

glm::vec3 lineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd);