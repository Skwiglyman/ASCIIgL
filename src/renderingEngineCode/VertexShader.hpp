#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

#include <vector>
#include <iostream>

#include "Vertex.hpp"

typedef struct Vertex_Shader
{
	glm::mat4 GLmodel = glm::mat4(1.0f);
	glm::mat4 GLview = glm::mat4(1.0f);
	glm::mat4 GLproj = glm::mat4(1.0f);

	void GLUse(VERTEX& vertice)
	{
		vertice.SetXYZW(GLproj * GLview * GLmodel * vertice.GetXYZW());
	}
} VERTEX_SHADER;