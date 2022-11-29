#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera3D
{
private:

public:
	glm::vec3 position;
	glm::mat4 view;
	glm::mat4 proj;

	float fov;
	float yaw;
	float pitch;
	float aspect;

	float zNear;
	float zFar;

	Camera3D(glm::vec3 Pposition, float Pfov, float Paspect, glm::vec2 yawPitch, float PzNear, float PzFar);
	~Camera3D();

	glm::vec3 getCamFront();
	glm::vec3 getCamBack();
	glm::vec3 getCamRight();
	glm::vec3 getCamLeft();
	

	void setCamPos(glm::vec3 Pposition);
	void setCamDir(float yaw, float pitch);
	void recalculateViewMat();
	void mouseRotation();
};