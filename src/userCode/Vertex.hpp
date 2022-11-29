#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

typedef struct Vertex // this is a struct that represents all vertexes
{
	union
	{
		glm::vec3 pos;
		glm::vec3 Position;
	};
	
} VERTEX;