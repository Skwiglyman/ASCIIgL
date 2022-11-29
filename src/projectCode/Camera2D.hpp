#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera2D
{
private:

public:
	glm::vec2 position;
	glm::mat4 view;
	glm::mat4 proj;

	Camera2D(glm::vec2 Pposition, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
	~Camera2D();

	void setCamPos(glm::vec2 Pposition);
	void recalculateViewMat();
};