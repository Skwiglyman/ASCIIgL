#pragma once

// includes from downloaded libraries
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// includes from default c++ libraries
#include <Windows.h>
#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>

// I don't even know what this does black magic screen shit
#define WIN32_LEAN_AND_MEAN


class Screen
{
private:
	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE); // handle thing (I got no idea what this is)

	COORD dwBufferSize; // size of buffer
	COORD dwBufferCoord; // starting coord of buffer
	SMALL_RECT rcRegion; // rectangle of coords of buffer { 0, 0, width - 1, height - 1 }

public:
	CHAR_INFO* pixelBuffer; // pixel output buffer

	int SCR_WIDTH, SCR_HEIGHT; // defining the width and height of the screen

	Screen(int width, int height); 
	~Screen();

	void ClearScreen(); // clears screen (does not clear buffer)
	void ClearBuffer(); // clears buffer
	void OutputBuffer(); // draws to screen
	void PlotPixel(glm::vec2 p); // plotting pixel onto screen buffer
	void DrawLine(int x1, int y1, int x2, int y2);
	glm::vec3 clipToScreen(glm::vec4 p);
	
	template<typename T> void RenderTriangles(T& VERTEX_SHADER, std::vector<std::vector<float>> vertices)
	{
		std::vector<std::vector<float>> transformed;

		for (int i = 0; i < vertices.size(); i++)
		{
			// transforming vertices using vertex shader
			glm::vec4 newVert = VERTEX_SHADER.Use(vertices[i]);

			// perspective division
			newVert.x /= newVert.w;
			newVert.y /= newVert.w;
			newVert.z /= newVert.w;


			// changing from clip space to screen space and adding it to transformed coords
			glm::vec3 screenCoords = clipToScreen(newVert);
			transformed.push_back({ screenCoords.x, screenCoords.y, screenCoords.z });
		}

		if (transformed.size() >= 3)
		{
			for (size_t i = 0; i < transformed.size(); i+=3)
			{
				DrawLine(transformed[i][0], transformed[i][1], transformed[i + 1][0], transformed[i + 1][1]);
				DrawLine(transformed[i + 1][0], transformed[i + 1][1], transformed[i + 2][0], transformed[i + 2][1]);
				DrawLine(transformed[i + 2][0], transformed[i + 2][1], transformed[i][0], transformed[i][1]);
			}
		}

		DrawLine(0, 0, SCR_WIDTH - 1, 0);
		DrawLine(SCR_WIDTH - 1, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1);
		DrawLine(SCR_WIDTH - 1, SCR_HEIGHT - 1, 0, SCR_HEIGHT - 1);
		DrawLine(0, 0, 0, SCR_HEIGHT - 1);
	}

	typedef struct Def_Vertex_Shader
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;

		glm::vec4 Use(std::vector<float> vertice)
		{
			glm::vec4 pos = glm::vec4(vertice[0], vertice[1], vertice[2], 1.0f);

			return proj * view * model * pos;
		}
	} DEF_VERTEX_SHADER;
};