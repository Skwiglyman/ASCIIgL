#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera3D // This is an inbuilt camera 3d class that primarily holds a view mat and a perspective proj mat to make a 3d camera
{
private:

public:
	glm::vec3 pos;
	glm::mat4 view; // this holds the view matrix (the view matrix is a glm::lookat matrix which is used for cameras)
	glm::mat4 proj; // this holds the perspective projection matrix

	float fov; // field of view for the camera, used in the perspective projection matrix

	// rotations of the camera
	float yaw;
	float pitch;
	float aspect;

	// clipping planes on the z axis for the camera
	float zNear;
	float zFar;

	// constructor and destructor
	Camera3D(glm::vec3 Pposition, float Pfov, float Paspect, glm::vec2 yawPitch, float PzNear, float PzFar);
	~Camera3D();

	// each of these output 3d vectors in the front, back, right and left direction
	glm::vec3 getCamFront();
	glm::vec3 getCamBack();
	glm::vec3 getCamRight();
	glm::vec3 getCamLeft();

	void setCamPos(glm::vec3 Pposition);
	void setCamDir(float yaw, float pitch); // this sets the camera dir using angles (not vectors)
	void setCamDir(glm::vec3 dir);
	void recalculateViewMat(); // recalculates the view matrix using the cameras euler angles and position
};