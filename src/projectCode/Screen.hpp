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
#include <chrono>

// I don't even know what this does black magic screen shit
#define WIN32_LEAN_AND_MEAN

// includes from other project files
#include "Camera3D.hpp"
#include "VertexShader.hpp"
#include "Vertex.hpp"
#include "ASCIIgLEngine.h"	

class Screen
{
private:
	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE); // handle thing (I got no idea what this is)

	COORD dwBufferSize; // size of buffer
	COORD dwBufferCoord; // starting coord of buffer
	SMALL_RECT rcRegion; // rectangle of coords of buffer { 0, 0, width - 1, height - 1 }

	void SetTitle();

	Screen();
	static inline Screen* Instance = nullptr;
	
public:
	CHAR_INFO* pixelBuffer; // pixel output buffer
	int SCR_WIDTH;
	int SCR_HEIGHT; // defining the width and height of the screen
	std::wstring SCR_TITLE;
	float fElapsedTime;

	static Screen* GetInstance();
	void InitializeScreen(int width, int height, std::wstring title);

	Screen(const Screen& obj) = delete;
	~Screen();

	VERTEX ViewPortTransform(VERTEX vertice); // changes clip space to screen space coords

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

	void RenderTriangles(VERTEX_SHADER VSHADER, std::vector<VERTEX> vertices)
	{
		// check if there are enough vertices to make a triangle or enough vertice data (at least xyz)
		if (vertices.size() < 3 || vertices.size() % 3 != 0)
			return;

		std::vector<VERTEX> CLIPPED_COORDS;

		for (int i = 0; i < vertices.size(); i += 3)
		{
			// VERTEX TRANSFORMING
			for (int k = 0; k < 3; k++)
				vertices[k + i] = VSHADER.GLUse(vertices[k + i]);

		
			// CLIPPING
			ClippingHelper(vertices, CLIPPED_COORDS, i);
		}

		for (int i = 0; i < CLIPPED_COORDS.size(); i += 3)
		{
			
			for (int k = 0; k < 3; k++)
			{
				// PERSPECTIVE DIVISION
				PerspectiveDivision(CLIPPED_COORDS, i + k);
				CLIPPED_COORDS[i + k] = ViewPortTransform(CLIPPED_COORDS[i + k]);
			}	

			if (BACKFACECULLING == true)
			{
				if (BackFaceCull(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], true))
				{
					if (WIREFRAME == true)
						DrawTriangleWireFrame(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2]);
					else
						// DRAWING TRIANGLES
						DrawTriangleFill(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2]);
				}
			}
			else
			{
				if (WIREFRAME == true)
					DrawTriangleWireFrame(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2]);
				else
					// DRAWING TRIANGLES
					DrawTriangleFill(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2]);
			}

		}

		// DRAWING BORDERS
		DrawLine(0, 0, SCR_WIDTH - 1, 0, PIXEL_SOLID, FG_WHITE);
		DrawLine(SCR_WIDTH - 1, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1, PIXEL_SOLID, FG_WHITE);
		DrawLine(SCR_WIDTH - 1, SCR_HEIGHT - 1, 0, SCR_HEIGHT - 1, PIXEL_SOLID, FG_WHITE);
		DrawLine(0, 0, 0, SCR_HEIGHT - 1, PIXEL_SOLID, FG_WHITE);
	}
};