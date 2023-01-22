#pragma once

// includes from downloaded libraries
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// I don't even know what this black magic screen shit does
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// includes from default c++ libraries
#include <Windows.h>
#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>

// includes from other project files
#include "VertexShader.hpp"
#include "Vertex.hpp"
#include "ASCIIgLEngine.hpp"	
#include "Texture.hpp"

#define NOERROR 1
#define WIN_WIDTH_TOO_BIG 2
#define WIN_HEIGHT_TOO_BIG 3


class Screen
{
private:
	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE); // handle thing (I got no idea what this is)

	COORD dwBufferSize; // size of buffer
	COORD dwBufferCoord; // starting coord of buffer
	SMALL_RECT rcRegion; // rectangle of coords of buffer { 0, 0, width - 1, height - 1 }

	Screen();
	static inline Screen* Instance = nullptr;

	inline static std::chrono::system_clock::time_point startTimeFps = std::chrono::system_clock::now();
	inline static std::chrono::system_clock::time_point endTimeFps = std::chrono::system_clock::now();

	void DrawLine(int x1, int y1, int x2, int y2, CHAR pixel_type, short col);
	void DrawTriangleWireFrame(VERTEX v1, VERTEX v2, VERTEX v3, CHAR pixel_type, short col);
	void DrawTriangleFill(VERTEX v1, VERTEX v2, VERTEX v3, CHAR pixel_type, short col);
	void DrawTriangleTextured(VERTEX vert1, VERTEX vert2, VERTEX vert3, Texture* tex);
	
public:
	CHAR_INFO* pixelBuffer; // pixel output buffer
	glm::vec3* colourBuffer; // stores the rgb colour of every pixel on the screen
	float* depthBuffer;

	// defining the width and height of the screen
	int SCR_WIDTH;
	int SCR_HEIGHT; 

	std::wstring SCR_TITLE;
	
	static inline float elapsedTime = 0;

	unsigned int fontW;
	unsigned int fontH;

	static Screen* GetInstance();
	int InitializeScreen(unsigned int width, unsigned int height, const std::wstring title, unsigned int fontX, unsigned int fontY);

	Screen(const Screen& obj) = delete;
	~Screen();

	VERTEX ViewPortTransform(VERTEX vertice); // changes clip space to screen space coords
	glm::vec3 BlendRGB(glm::vec4 inRGB, glm::vec2 pixelPos);

	void SetTitle();
	void StartFPSClock();
	void EndFPSClock();

	void ClearScreen(); // clears screen (does not clear buffer)
	void ClearBuffer(unsigned short backgrounCol); // clears buffer
	void OutputBuffer(); // draws to screen
	void PlotPixel(glm::vec2 p, CHAR character, short Colour); // plotting pixel onto screen buffer
	void PlotPixel(glm::vec2 p, CHAR_INFO charCol);
	void DrawBorder(short col);

	float GetDeltaTime();

	bool WIREFRAME = true; // flag that determines whether triangles are drawn normally or using wireframe
	bool BACKFACECULLING = true; // flag that determines whether backface culling is done
	bool CCW = true; // counter clockwise vertice winding order
	bool BLEND = false;

	void RenderTriangles(VERTEX_SHADER VSHADER, std::vector<VERTEX> vertices, Texture* tex)
	{
		// check if there are enough vertices to make a triangle or enough vertice data (at least xyz)
		if (vertices.size() < 3 || vertices.size() % 3 != 0)
			return;

		for (int k = 0; k < vertices.size(); k++) 
		{ 
			VSHADER.GLUse(vertices[k]); 
		} // VERTEX TRANSFORMING


		// CLIPPING
		std::vector<VERTEX> CLIPPED_COORDS;
		ASCIIgLEngine::ClippingHelper(vertices, CLIPPED_COORDS);

		for (int i = 0; i < CLIPPED_COORDS.size(); i += 3)
		{
			for (int k = 0; k < 3; k++)
			{
				// PERSPECTIVE DIVISION
				ASCIIgLEngine::PerspectiveDivision(CLIPPED_COORDS, i + k);
				CLIPPED_COORDS[i + k] = ViewPortTransform(CLIPPED_COORDS[i + k]);
				CLIPPED_COORDS[i + k].refactorPtrs();
			}	
		
			if ((BACKFACECULLING == true ? ASCIIgLEngine::BackFaceCull(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], CCW) : true))
			{
				if (WIREFRAME == true or tex == nullptr) { DrawTriangleWireFrame(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], PIXEL_SOLID, FG_WHITE); }
				else { DrawTriangleTextured(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], tex); }
			}
		}
	}
};