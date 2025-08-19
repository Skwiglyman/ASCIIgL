#pragma once

// includes from downloaded libraries
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include <engine/GameObj.hpp>

class Present : public GameObj
{
private:

public:
	bool collected = false;

	glm::vec2 rotation = glm::vec2(0, 0);

	Present(glm::vec3 pos, glm::vec3 size, Model* model)
		: GameObj(pos, rotation, size, model)
	{

	}
};