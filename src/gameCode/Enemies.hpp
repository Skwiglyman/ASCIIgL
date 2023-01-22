#pragma once

#include "../gameEngineCode/GameObj.hpp"
#include "../gameEngineCode/Model.hpp"

class Enemy: public GameObj
{
private:
	
public:
	glm::vec2 rotation = glm::vec2(0, 0);

	enum MODES
	{
		CHASE,
		PATROL
	};

	int aiState;
	glm::vec3 patrolDest;
	int destRadius = 10;

	glm::vec3 patrolStart;
	glm::vec3 patrolEnd;

	Enemy(glm::vec3 pos, glm::vec3 size, Model* model, int state = CHASE, glm::vec3 patrolS = glm::vec3(0,0,0), glm::vec3 patrolE = glm::vec3(0, 0, 0))
		: GameObj(pos, rotation, size, model), aiState(state), patrolDest(patrolS), patrolStart(patrolS), patrolEnd(patrolE)
	{

	}

	void Move(glm::vec3 move)
	{
		position += move;
	};
};
