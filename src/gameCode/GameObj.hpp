#pragma once

#include "Model.hpp"

class GameObj
{
private:

public:
	GameObj(glm::vec3 pos, glm::vec2 rot, glm::vec3 inSize, Model* modelPtr);
	~GameObj();

	Model* model = nullptr;
	glm::vec3 position;
	glm::vec2 rotation;
	glm::vec3 size;
};