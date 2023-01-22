#include "Camera3D.hpp"
#include <math.h>

Camera3D::Camera3D(glm::vec3 Pposition, float Pfov, float Paspect, glm::vec2 yawPitch, float PzNear, float PzFar)
	: pos(Pposition), fov(Pfov), pitch(yawPitch.y), yaw(yawPitch.x), aspect(Paspect), zNear(PzNear), zFar(PzFar)
{
	recalculateViewMat(); // this function uses all all class attributes
	proj = glm::perspective(glm::radians(Pfov), aspect, zNear, zFar); // calculating the perspective projection
}

Camera3D::~Camera3D()
{
	// unused destructor
}

glm::vec3 Camera3D::getCamFront()
{
	// this function calculates the directional vector of the front of the camera (where the camera is facing)
	glm::vec3 camDir(0.0f, 0.0f, 0.0f);

	// these are all random ass ways to translate euler angles to a directional vector online don't ask me how it works
	camDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	camDir.y = sin(glm::radians(pitch));
	camDir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camDir = glm::normalize(camDir); // turning it into a direction instead of a 3d point
	
	return camDir;
}

glm::vec3 Camera3D::getCamBack()
{
	// this function returns the vector dir of the back of the camera by taking the reverse of the dir front of the camera
	return -getCamFront();
}

glm::vec3 Camera3D::getCamRight()
{
	// this function gets the right dir by getting the cross product of front and up vector (gets vector perpendicular to both, which is the right vector)
	return glm::normalize(glm::cross(getCamFront(), glm::vec3(0, 1, 0)));
}

glm::vec3 Camera3D::getCamLeft()
{
	// this function returns the vector dir of the left of the camera by taking the reverse of the dir right of the camera
	return -getCamRight();
}

void Camera3D::setCamPos(glm::vec3 Pposition)
{
	// this function takes in a new position for the camera, and sets the cameras position to it, then it recalculates the view mat
	pos = Pposition;
	recalculateViewMat();
}

void Camera3D::setCamDir(float Pyaw, float Ppitch)
{
	// this function takes in a yaw and pitch (no roll cus I didn't code that in (really should)) and sets the yaw in pitch to the new yaw and pitch
	yaw = Pyaw;
	pitch = Ppitch;

	if (pitch > 89.0f) // pitch limiting
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	recalculateViewMat();
}

void Camera3D::setCamDir(glm::vec3 dir)
{
	yaw = acos(dir.x / (-dir.y));
	pitch = sqrt(pow(dir.x, 2) + pow(dir.y, 2)) / dir.z;

	recalculateViewMat();
}

void Camera3D::recalculateViewMat()
{
	// sets the cameras view mat to a glm::look at matrix using the place the camera is facing as the position matrix + the direction of the camera
	view = glm::lookAt(pos, pos + getCamFront(), glm::vec3(0.0, 1.0, 0.0));
}