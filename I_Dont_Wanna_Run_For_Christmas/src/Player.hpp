#pragma once

// Game Engine Code Includes
#include <engine/Camera3D.hpp>
#include <engine/Mesh.hpp>
#include <engine/GameObj.hpp>

// Rendering Engine Code Includes
#include <renderer/Screen.hpp>
#include <renderer/ASCIIgLEngine.hpp>

class Player // this class represents the player, its not a singleton because I was too lazy to make it one
{
private:
	static inline float nearClip = 1.0f;   	   // honestly I just want this project over at this point this code is so bad
	static inline float farClip = 2000.0f;	   // why am I making this static inline idk but I guess I am
	static inline float fov = 80;			   // anyway these are just camera settings for the player

public:
	Player(glm::vec2 xz, glm::vec2 yawPitch); // player has no y input because you cant go up or down in the game
	~Player();

	glm::vec3 GetPlayerPos(); // just simplifies getting position
	glm::vec3 GetMovement(); // gets input and spits out a movement vector that it wants to do
	glm::vec2 GetViewChange(); // gets input and spits out a new direction the camera looks at
	glm::vec3 Sprinting(glm::vec3 move); // this handles all of the sprinting logic

	bool CollideLevel(glm::vec3 move, GameObj* Level); // handles all of the collision with the outside walls of the level

	void Update(GameObj* Level); // just a general update method that runs everything for the player

	// fps camera
	Camera3D camera;

	// stamina system
	static inline float maxStamina = 500;
	static inline float stamina = maxStamina;
	static inline float staminaRegen = 2;
	static inline float sprintFactor = 2;
	static inline float staminaLoss = 3;
	static inline bool tired = false;
	
	static inline float playerHeight = 20;
	static inline float playerHitBoxRad = 15;
	

};