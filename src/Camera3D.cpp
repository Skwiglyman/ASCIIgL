#include "Camera3D.hpp"
#include <math.h>

Camera3D::Camera3D(glm::vec3 Pposition, float Pfov, float Paspect, glm::vec2 yawPitch, float PzNear, float PzFar)
	: position(Pposition), fov(Pfov), pitch(yawPitch.y), yaw(yawPitch.x), aspect(Paspect), zNear(PzNear), zFar(PzFar)
{
	recalculateViewMat();
	proj = glm::perspective(glm::radians(Pfov), aspect, zNear, zFar);
}

Camera3D::~Camera3D()
{

}

glm::vec3 Camera3D::getCamFront()
{
	glm::vec3 camDir(0.0f, 0.0f, 0.0f);
	camDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	camDir.y = sin(glm::radians(pitch));
	camDir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camDir = glm::normalize(camDir);
	
	return camDir;
}

glm::vec3 Camera3D::getCamBack()
{
	return -getCamFront();
}

glm::vec3 Camera3D::getCamRight()
{
	return glm::normalize(glm::cross(getCamFront(), glm::vec3(0, 1, 0)));
}

glm::vec3 Camera3D::getCamLeft()
{
	return -getCamRight();
}

void Camera3D::setCamPos(glm::vec3 Pposition)
{
	position = Pposition;
	recalculateViewMat();
}

void Camera3D::setCamDir(float Pyaw, float Ppitch)
{
	yaw = Pyaw;
	pitch = Ppitch;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	recalculateViewMat();
}

void Camera3D::recalculateViewMat()
{
	view = glm::lookAt(position, position + getCamFront(), glm::vec3(0.0, 1.0, 0.0));
}