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
#include <random>

// I don't even know what this does black magic screen shit
#define WIN32_LEAN_AND_MEAN

// includes from other project files
#include "Camera3D.hpp"
#include "ASCII_Math.hpp"
#include "VertexShader.hpp"

class Screen
{
private:
	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE); // handle thing (I got no idea what this is)

	COORD dwBufferSize; // size of buffer
	COORD dwBufferCoord; // starting coord of buffer
	SMALL_RECT rcRegion; // rectangle of coords of buffer { 0, 0, width - 1, height - 1 }
	
	std::vector<float> clipToScreen(std::vector<float> p); // changes clip space to screen space coords
	void clipTriangleAgainstPlane(glm::vec3 planeP, glm::vec3 planeN, std::vector<std::vector<float>>& vertices, std::vector<std::vector<float>>& clipped, int i);
	float BackFaceCull(std::vector<float> v1, std::vector<float> v2, std::vector<float> v3);

	void DrawTriangles(VERTEX_SHADER VSHADER, std::vector<std::vector<float>> screenCoords);

	void localToWorld(VERTEX_SHADER VSHADER, std::vector<std::vector<float>>& localCoords, std::vector<std::vector<float>>& worldCoords);
	void worldToView(VERTEX_SHADER VSHADER, std::vector<std::vector<float>>& worldCoords, std::vector<std::vector<float>>& viewCoords);
	void viewToClip(VERTEX_SHADER VSHADER, std::vector<std::vector<float>>& viewCoords, std::vector<std::vector<float>>& clipCoords);
	void perspectiveDivision(std::vector<std::vector<float>>& clipCoords);
	void NDCToScreen(std::vector<std::vector<float>>& ndcCoords, std::vector<std::vector<float>>& screenCoords);

	void depthClipping(std::vector<std::vector<float>>& viewCoords, std::vector<std::vector<float>>& clippedViewCoords, float zNear, float zFar);
	void backFaceCulling(std::vector<std::vector<float>>& clippedViewCoords, std::vector<std::vector<float>>& backClippedCoords);
	void viewClipping(std::vector<std::vector<float>>& screenCoords, std::vector<std::vector<float>>& toDrawCoords);
	


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

	void RenderTriangles(VERTEX_SHADER VSHADER, std::vector<std::vector<float>> localCoords, float zNear, float zFar)
	{
		// check if there are enough vertices to make a triangle or enough vertice data (at least xyz)
		if (localCoords.size() < 3 || localCoords.size() % 3 != 0)
			return;
		else if (localCoords[0].size() < 3)
			return;

		// LOCAL TO WORLD
		std::vector<std::vector<float>> worldCoords;
		localToWorld(VSHADER, localCoords, worldCoords);

		// WORLD TO VIEW
		std::vector<std::vector<float>> viewCoords;
		worldToView(VSHADER, worldCoords, viewCoords);

		// CLIPPING
		std::vector<std::vector<float>> clippedViewCoords;
		depthClipping(viewCoords, clippedViewCoords, zNear, zFar);

		std::vector<std::vector<float>> backClippedCoords;
		backFaceCulling(clippedViewCoords, backClippedCoords);

		// VIEW TO CLIP
		std::vector<std::vector<float>> clipCoords;
		viewToClip(VSHADER, backClippedCoords, clipCoords);

		// PERSPECTIVE DIVISION
		perspectiveDivision(clipCoords);

		// NDC TO SCREEN
		std::vector<std::vector<float>> screenCoords;
		NDCToScreen(clipCoords, screenCoords);

		// FINAL VIEW CLIPPING
		std::vector<std::vector<float>> toDrawCoords;
		viewClipping(screenCoords, toDrawCoords);

		// DRAWING TRIANGLES
		DrawTriangles(VSHADER, toDrawCoords);

		// DRAWING BORDERS
		DrawLine(0, 0, SCR_WIDTH - 1, 0);
		DrawLine(SCR_WIDTH - 1, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1);
		DrawLine(SCR_WIDTH - 1, SCR_HEIGHT - 1, 0, SCR_HEIGHT - 1);
		DrawLine(0, 0, 0, SCR_HEIGHT - 1);
	}
};