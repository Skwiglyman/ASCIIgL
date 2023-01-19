#include "GameObj.hpp"

GameObj::GameObj(glm::vec3 pos, glm::vec2 rot, glm::vec3 inSize, Model* modelPtr)
	: position(pos), rotation(rot), size(inSize)
{
	model = modelPtr;
}

GameObj::~GameObj()
{

}