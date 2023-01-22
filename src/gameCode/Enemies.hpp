#pragma once

#include "../gameEngineCode/GameObj.hpp"
#include "../gameEngineCode/Model.hpp"

class Enemy: public GameObj
{
private:
	
public:
	glm::vec2 rotation = glm::vec2(0, 0);

	Enemy(glm::vec3 pos, glm::vec3 size, Model* model)
		: GameObj(pos, rotation, size, model)
	{

	}

	void Move(glm::vec3 move)
	{
		position += move;
	};
};
