#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../gameEngineCode/Camera3D.hpp"
#include "../gameEngineCode/Mesh.hpp"
#include "../gameEngineCode/GameObj.hpp"

#include "../renderingEngineCode/Screen.hpp"
#include "../renderingEngineCode/ASCIIgLEngine.hpp"

class Player
{
private:
	float nearClip = 1.0f;
	float farClip = 2000.0f;
	float fov = 80;
	float playerHitBoxRad = 25.0f;
	float playerHeight = 20;

public:
	Player(glm::vec2 xz, glm::vec2 yawPitch);
	~Player();

	glm::vec3 GetPlayerPos();
	glm::vec3 GetMovement();
	glm::vec2 GetViewChange();

	bool CollideLevel(glm::vec3 move, GameObj* Level);

	void Update(GameObj* Level);

	Camera3D camera;
	int presentsCollected = 0;

};