#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

#include "ASCII_Math.hpp"

typedef struct Vertex_Shader
{
	glm::mat4 GLmodel;
	glm::mat4 GLview;
	glm::mat4 GLproj;

	std::vector<float> GLlocalToWorld(std::vector<float> vertice)
	{
		std::vector<float> newVert;
		glm::vec4 newPos = GLmodel * glm::vec4(vertice[0], vertice[1], vertice[2], 1.0f);
		vec4ToVert(newPos, &newVert);

		return newVert;
	}

	std::vector<float> GLworldToView(std::vector<float> vertice)
	{
		std::vector<float> newVert;
		glm::vec4 newPos = GLview * glm::vec4(vertice[0], vertice[1], vertice[2], 1.0f);
		vec4ToVert(newPos, &newVert);

		return newVert;
	}

	std::vector<float> GLviewToClip(std::vector<float> vertice)
	{
		std::vector<float> newVert;
		glm::vec4 newPos;

		if (vertice[2] == 0)
			newPos = GLproj * glm::vec4(vertice[0], vertice[1], vertice[2] + 0.01, 1.0f);
		else
			newPos = GLproj * glm::vec4(vertice[0], vertice[1], vertice[2], 1.0f);
		vec4ToVert(newPos, &newVert);

		return newVert;
	}
} VERTEX_SHADER;