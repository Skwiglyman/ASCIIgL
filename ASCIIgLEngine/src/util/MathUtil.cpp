#include "MathUtil.hpp"

glm::vec3 MathUtil::CalcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, bool out)
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