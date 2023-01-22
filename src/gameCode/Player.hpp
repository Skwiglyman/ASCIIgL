#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../gameEngineCode/Camera3D.hpp"
#include "../gameEngineCode/Mesh.hpp"
#include "../gameEngineCode/GameObj.hpp"

#include "../renderingEngineCode/Screen.hpp"
#include "../renderingEngineCode/ASCIIgLEngine.hpp"

#include "Game.hpp"

class Player
{
private:
	static inline float nearClip = 1.0f;
	static inline float farClip = 2000.0f;
	static inline float fov = 80;
	

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
	
	static inline float playerHeight = 20;
	static inline float playerHitBoxRad = 25.0f;
	

};