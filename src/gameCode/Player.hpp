#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Camera3D.hpp"
#include "Mesh.hpp"
#include "GameObj.hpp"

#include "../renderingEngineCode/Screen.hpp"

class Player
{
private:
	float nearClip = 1.0f;
	float farClip = 300.0f;
	float fov = 80;

public:
	Player(glm::vec2 xz, glm::vec2 yawPitch, float playerHeight);
	~Player();

	glm::vec3 GetPlayerPos();
	glm::vec3 GetMovement();
	glm::vec2 GetViewChange();

	void Update(std::vector<GameObj*> obstacles);

	Camera3D camera;
	int presentsCollected = 0;

};