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
#include "Vertex.hpp"
#include "Colour.hpp"

class Screen
{
private:
	// Unused Functions
	//void ViewClippingHelper(std::vector<VERTEX>& screenCoords, std::vector<VERTEX>& toDrawCoords);
	//void ViewClipping(glm::vec3 planeP, glm::vec3 planeN, std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped);

	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE); // handle thing (I got no idea what this is)

	COORD dwBufferSize; // size of buffer
	COORD dwBufferCoord; // starting coord of buffer
	SMALL_RECT rcRegion; // rectangle of coords of buffer { 0, 0, width - 1, height - 1 }
	
	VERTEX ClipToScreen(VERTEX vertice); // changes clip space to screen space coords
	void Clipping(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped, int component, bool Near);
	bool BackFaceCull(VERTEX v1, VERTEX v2, VERTEX v3);

	void DrawTrianglesWireFrame(std::vector<VERTEX> screenCoords);
	void DrawTrianglesFill(std::vector<VERTEX> screenCoords);

	void VertexTransform(VERTEX_SHADER VSHADER, std::vector<VERTEX>& localCoords, std::vector<VERTEX>& clipCoords);
	void NDCToScreen(std::vector<VERTEX>& ndcCoords, std::vector<VERTEX>& screenCoords);

	void PerspectiveDivision(std::vector<VERTEX>& clipCoords);
	void BackFaceCulling(std::vector<VERTEX>& clippedViewCoords, std::vector<VERTEX>& backClippedCoords);

	void ClippingHelper(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped);

public:
	CHAR_INFO* pixelBuffer; // pixel output buffer

	int SCR_WIDTH, SCR_HEIGHT; // defining the width and height of the screen

	Screen(int width, int height); 
	~Screen();

	void ClearScreen(); // clears screen (does not clear buffer)
	void ClearBuffer(); // clears buffer
	void OutputBuffer(); // draws to screen
	void PlotPixel(glm::vec2 p, CHAR character, short Colour); // plotting pixel onto screen buffer

	void DrawLine(int x1, int y1, int x2, int y2, CHAR pixel_type, short col);
	void DrawTriangleWireFrame(VERTEX v1, VERTEX v2, VERTEX v3);
	void DrawTriangleFill(VERTEX v1, VERTEX v2, VERTEX v3);

	bool WIREFRAME = true; // flag that determines whether triangles are drawn normally or using wireframe
	bool BACKFACECULLING = true; // flag that determines whether backface culling is done
	bool CCW = true; // counter clockwise vertice winding order

	// symbol and colour are 2 last

	void RenderTriangles(VERTEX_SHADER VSHADER, std::vector<VERTEX> localCoords)
	{
		// check if there are enough vertices to make a triangle or enough vertice data (at least xyz)
		if (localCoords.size() < 3 || localCoords.size() % 3 != 0)
			return;

		// VERTEX TRANSFORMING
		std::vector<VERTEX> CS_COORDS;
		VertexTransform(VSHADER, localCoords, CS_COORDS);

		// CLIPPING
		std::vector<VERTEX> CLIPPED_CS_COORDS;
		ClippingHelper(CS_COORDS, CLIPPED_CS_COORDS);

		// PERSPECTIVE DIVISION
		PerspectiveDivision(CLIPPED_CS_COORDS);

		// NDC TO SCREEN
		std::vector<VERTEX> SS_COORDS;
		NDCToScreen(CLIPPED_CS_COORDS, SS_COORDS);

		// BACKFACE CULLING 
		std::vector<VERTEX> BACKCULLED_SS_COORDS;
		if (BACKFACECULLING == true)
			BackFaceCulling(SS_COORDS, BACKCULLED_SS_COORDS);
		else
			BACKCULLED_SS_COORDS = SS_COORDS;

		// DRAWING TRIANGLES
		if (WIREFRAME == true)
			DrawTrianglesWireFrame(BACKCULLED_SS_COORDS);
		else
			DrawTrianglesFill(BACKCULLED_SS_COORDS);

		// DRAWING BORDERS
		DrawLine(0, 0, SCR_WIDTH - 1, 0, PIXEL_SOLID, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		DrawLine(SCR_WIDTH - 1, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1, PIXEL_SOLID, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		DrawLine(SCR_WIDTH - 1, SCR_HEIGHT - 1, 0, SCR_HEIGHT - 1, PIXEL_SOLID, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		DrawLine(0, 0, 0, SCR_HEIGHT - 1, PIXEL_SOLID, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
};