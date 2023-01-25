#pragma once

// includes from downloaded libraries
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

// includes from default c++ libraries
#include <vector>
#include <iostream>

// includes from the rendering engine
#include "Vertex.hpp"

typedef struct Vertex_Shader // this just represents a program that can have its variables pre programmed from the outside
{
	glm::mat4 GLmodel = glm::mat4(1.0f);
	glm::mat4 GLview = glm::mat4(1.0f);
	glm::mat4 GLproj = glm::mat4(1.0f);

	void GLUse(VERTEX& vertice) // the vertex shader takes in a vertice in local space and spits it out in clip space before perspective divide
	{
		vertice.SetXYZW(GLproj * GLview * GLmodel * vertice.GetXYZW());
	}
} VERTEX_SHADER;