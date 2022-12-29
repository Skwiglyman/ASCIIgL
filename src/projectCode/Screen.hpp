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
	void viewClipping(glm::vec3 planeP, glm::vec3 planeN, std::vector<std::vector<float>>& vertices, std::vector<std::vector<float>>& clipped, int i);
	void nearClipping(std::vector<std::vector<float>>& clipCoords, std::vector<std::vector<float>>& clippedViewCoords);
	bool BackFaceCull(std::vector<float> v1, std::vector<float> v2, std::vector<float> v3);

	void DrawTrianglesWireFrame(VERTEX_SHADER VSHADER, std::vector<std::vector<float>> screenCoords);
	void DrawTrianglesFill(VERTEX_SHADER VSHADER, std::vector<std::vector<float>> screenCoords);

	void localToWorld(VERTEX_SHADER VSHADER, std::vector<std::vector<float>>& localCoords, std::vector<std::vector<float>>& worldCoords);
	void worldToView(VERTEX_SHADER VSHADER, std::vector<std::vector<float>>& worldCoords, std::vector<std::vector<float>>& viewCoords);
	void viewToClip(VERTEX_SHADER VSHADER, std::vector<std::vector<float>>& viewCoords, std::vector<std::vector<float>>& clipCoords);
	void NDCToScreen(std::vector<std::vector<float>>& ndcCoords, std::vector<std::vector<float>>& screenCoords);

	void perspectiveDivision(std::vector<std::vector<float>>& clipCoords);
	void backFaceCulling(std::vector<std::vector<float>>& clippedViewCoords, std::vector<std::vector<float>>& backClippedCoords);
	void viewClippingHelper(std::vector<std::vector<float>>& screenCoords, std::vector<std::vector<float>>& toDrawCoords);

	enum COLOUR
	{
		FG_BLACK = 0x0000,
		FG_DARK_BLUE = 0x0001,
		FG_DARK_GREEN = 0x0002,
		FG_DARK_CYAN = 0x0003,
		FG_DARK_RED = 0x0004,
		FG_DARK_MAGENTA = 0x0005,
		FG_DARK_YELLOW = 0x0006,
		FG_GREY = 0x0007,
		FG_DARK_GREY = 0x0008,
		FG_BLUE = 0x0009,
		FG_GREEN = 0x000A,
		FG_CYAN = 0x000B,
		FG_RED = 0x000C,
		FG_MAGENTA = 0x000D,
		FG_YELLOW = 0x000E,
		FG_WHITE = 0x000F,
		BG_BLACK = 0x0000,
		BG_DARK_BLUE = 0x0010,
		BG_DARK_GREEN = 0x0020,
		BG_DARK_CYAN = 0x0030,
		BG_DARK_RED = 0x0040,
		BG_DARK_MAGENTA = 0x0050,
		BG_DARK_YELLOW = 0x0060,
		BG_GREY = 0x0070,
		BG_DARK_GREY = 0x0080,
		BG_BLUE = 0x0090,
		BG_GREEN = 0x00A0,
		BG_CYAN = 0x00B0,
		BG_RED = 0x00C0,
		BG_MAGENTA = 0x00D0,
		BG_YELLOW = 0x00E0,
		BG_WHITE = 0x00F0,
	};
	
	enum PIXEL_TYPE
	{
		PIXEL_SOLID = 35,
		PIXEL_THREEQUARTERS = 120,
		PIXEL_HALF = 47,
		PIXEL_QUARTER = 45,
	};


public:
	CHAR_INFO* pixelBuffer; // pixel output buffer

	int SCR_WIDTH, SCR_HEIGHT; // defining the width and height of the screen

	Screen(int width, int height); 
	~Screen();

	void ClearScreen(); // clears screen (does not clear buffer)
	void ClearBuffer(); // clears buffer
	void OutputBuffer(); // draws to screen
	void PlotPixel(glm::vec2 p, CHAR character, short Colour); // plotting pixel onto screen buffer

	void DrawLine(int x1, int y1, int x2, int y2, short pixel_type, short col);
	void DrawTriangleWireFrame(std::vector<float> v1, std::vector<float> v2, std::vector<float> v3);
	void DrawTriangleFill(std::vector<float> v1, std::vector<float> v2, std::vector<float> v3);

	bool WIREFRAME = true; // flag that determines whether triangles are drawn normally or using wireframe
	bool BACKFACECULLING = true; // flag that determines whether backface culling is done
	bool CCW = true; // counter clockwise vertice winding order

	// symbol and colour are 2 last

	void RenderTriangles(VERTEX_SHADER VSHADER, std::vector<std::vector<float>> localCoords, float zNear, float zFar)
	{
		// check if there are enough vertices to make a triangle or enough vertice data (at least xyz)
		if (localCoords.size() < 3 || localCoords.size() % 3 != 0)
			return;
		else if (localCoords[0].size() < 3)
			return;

		// LOCAL TO WORLD
		std::vector<std::vector<float>> WS_COORDS;
		localToWorld(VSHADER, localCoords, WS_COORDS);

		// WORLD TO VIEW
		std::vector<std::vector<float>> VS_COORDS;
		worldToView(VSHADER, WS_COORDS, VS_COORDS);

		// VIEW TO CLIP
		std::vector<std::vector<float>> CS_COORDS;
		viewToClip(VSHADER, VS_COORDS, CS_COORDS);

		//// CLIPPING
		std::vector<std::vector<float>> NEAR_CLIPPED_CS_COORDS;
		nearClipping(CS_COORDS, NEAR_CLIPPED_CS_COORDS);

		// PERSPECTIVE DIVISION
		perspectiveDivision(NEAR_CLIPPED_CS_COORDS);

		// NDC TO SCREEN
		std::vector<std::vector<float>> SS_COORDS;
		NDCToScreen(NEAR_CLIPPED_CS_COORDS, SS_COORDS);

		// BACKFACE CULLING 
		std::vector<std::vector<float>> BACKCULLED_SS_COORDS;
		if (BACKFACECULLING == true)
			backFaceCulling(SS_COORDS, BACKCULLED_SS_COORDS);
		else
			BACKCULLED_SS_COORDS = SS_COORDS;

		std::vector<std::vector<float>> CLIPPED_SS_COORDS;
		viewClippingHelper(BACKCULLED_SS_COORDS, CLIPPED_SS_COORDS);

		for (int i = 0; i < CLIPPED_SS_COORDS.size(); i++)
		{
			CLIPPED_SS_COORDS[i].push_back(PIXEL_SOLID);
			CLIPPED_SS_COORDS[i].push_back(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}

		// DRAWING TRIANGLES
		if (WIREFRAME == true)
			DrawTrianglesWireFrame(VSHADER, CLIPPED_SS_COORDS);
		else
			DrawTrianglesFill(VSHADER, CLIPPED_SS_COORDS);

		// DRAWING BORDERS
		DrawLine(0, 0, SCR_WIDTH - 1, 0, PIXEL_SOLID, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		DrawLine(SCR_WIDTH - 1, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1, PIXEL_SOLID, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		DrawLine(SCR_WIDTH - 1, SCR_HEIGHT - 1, 0, SCR_HEIGHT - 1, PIXEL_SOLID, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		DrawLine(0, 0, 0, SCR_HEIGHT - 1, PIXEL_SOLID, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
};