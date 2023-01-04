#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

#include "Vertex.hpp"

typedef struct Vertex_Shader
{
	glm::mat4 GLmodel;
	glm::mat4 GLview;
	glm::mat4 GLproj;

	VERTEX GLUse(VERTEX vertice)
	{
		VERTEX newVert = vertice;
		glm::vec4 newPos = GLproj * GLview * GLmodel * vertice.GetXYZW();
		newVert.SetXYZW(newPos);
		return newVert;
	}
} VERTEX_SHADER;