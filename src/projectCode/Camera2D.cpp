#include "Camera2D.hpp"

Camera2D::Camera2D(glm::vec2 Pposition, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT)
	: position(Pposition)
{
	proj = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -100.0f, 100.0f);
	recalculateViewMat();
}

Camera2D::~Camera2D()
{

}

void Camera2D::setCamPos(glm::vec2 Pposition)
{
	position = Pposition;
	recalculateViewMat();
}

void Camera2D::recalculateViewMat()
{
	view = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));
}