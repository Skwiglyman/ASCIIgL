#pragma once

// includes from downloaded libraries
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

// I don't even know what this black magic screen shit does

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

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
#include "engine/Logger.hpp"

// defining error codes for screen initialization
#ifndef NOERROR
#define NOERROR 1
#endif
#define WIN_WIDTH_TOO_BIG 2
#define WIN_HEIGHT_TOO_BIG 3


class Screen // this class will represent the window itself, in retrospect it has many things it shouldn't have, but thats okay, its also a singleton
{
private:
	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE); // handle thing (I got no idea what this is)

	COORD dwBufferSize; // size of buffer
	COORD dwBufferCoord; // starting coord of buffer
	SMALL_RECT rcRegion; // rectangle of coords of buffer { 0, 0, width - 1, height - 1 }
	
	// setting up singleton class functionality by removing constructor
	Screen();
	static inline Screen* Instance = nullptr;

	// initializing the fps clock
	inline static std::chrono::system_clock::time_point startTimeFps = std::chrono::system_clock::now();
	inline static std::chrono::system_clock::time_point endTimeFps = std::chrono::system_clock::now();

	void DrawLine(int x1, int y1, int x2, int y2, CHAR pixel_type, short col);
	void DrawTriangleWireFrame(VERTEX v1, VERTEX v2, VERTEX v3, CHAR pixel_type, short col);
	void DrawTriangleTextured(VERTEX vert1, VERTEX vert2, VERTEX vert3, Texture* tex);
	
public:
	CHAR_INFO* pixelBuffer = nullptr; // pixel output buffer
	glm::vec3* colourBuffer = nullptr; // stores the rgb colour of every pixel on the screen
	float* depthBuffer = nullptr;

	// defining the width and height of the screen
	int SCR_WIDTH;
	int SCR_HEIGHT; 

	// holds the title of the window itself
	std::wstring SCR_TITLE;
	
	static inline float elapsedTime = 0; // holds elapsed time used for calculating delta time and fps

	// holds font width and height
	unsigned int fontW;
	unsigned int fontH;

	// setting up singleton class functionality
	static Screen* GetInstance();
	int InitializeScreen(unsigned int width, unsigned int height, const std::wstring title, unsigned int fontX, unsigned int fontY);

	// deleting the copy constructor for singleton class
	Screen(const Screen& obj) = delete;
	~Screen();

	// transforms vertice from [-1, 1] to [0 - scr_dim]
	VERTEX ViewPortTransform(VERTEX vertice); // changes clip space to screen space coords
	glm::vec3 BlendRGB(glm::vec4 inRGB, glm::vec2 pixelPos); // blends colour based on their alpha value

	// these functions control fps and the setting of the title, the order they need to be called in is start, end, set title
	void SetTitle();
	void StartFPSClock();
	void EndFPSClock();

	void ClearScreen(); // clears screen (does not clear buffer)
	void ClearBuffer(unsigned short backgrounCol); // clears buffer
	void OutputBuffer(); // draws to screen
	void PlotPixel(glm::vec2 p, CHAR character, short Colour); // plotting pixel onto screen buffer
	void PlotPixel(glm::vec2 p, CHAR_INFO charCol);
	void DrawBorder(short col); // shortcut function that draws a border onto the screen of 1 tile width

	float GetDeltaTime(); // returns the elapsed time x desired fps (idk if this is a good system I literally made it up in a minute...)

	bool WIREFRAME = true; // flag that determines whether triangles are drawn normally or using wireframe
	bool BACKFACECULLING = true; // flag that determines whether backface culling is done
	bool CCW = true; // counter clockwise vertice winding order
	bool BLEND = false;

	// rendering routine that draws triangles onto the screen
	void RenderTriangles(VERTEX_SHADER VSHADER, std::vector<VERTEX> vertices, Texture* tex)
	{
		Logger::Debug("[DEBUG] RenderTriangles called. Vertices size: " + std::to_string(vertices.size()) + ", Texture: " + std::string(tex ? "yes" : "no"));
		// check if there are enough vertices to make a triangle or enough vertice data (at least xyz)
		if (vertices.size() < 3 || vertices.size() % 3 != 0) {
			Logger::Debug("[DEBUG] Not enough vertices to render triangles.");
			return;
		}

		Logger::Debug("[DEBUG] Transforming vertices.");
		for (int k = 0; k < vertices.size(); k++) 
		{ 
			VSHADER.GLUse(vertices[k]); 
		} // VERTEX TRANSFORMING

		Logger::Debug("[DEBUG] Clipping triangles.");
		std::vector<VERTEX> CLIPPED_COORDS;
		ASCIIgLEngine::ClippingHelper(vertices, CLIPPED_COORDS);
		Logger::Debug("[DEBUG] Clipped coords size: " + std::to_string(CLIPPED_COORDS.size()));
		for (int i = 0; i < CLIPPED_COORDS.size(); i += 3)
		{
			Logger::Debug("[DEBUG] Processing triangle at index " + std::to_string(i));
			for (int k = 0; k < 3; k++)
			{
				// PERSPECTIVE DIVISION
				ASCIIgLEngine::PerspectiveDivision(CLIPPED_COORDS, i + k);
				CLIPPED_COORDS[i + k] = ViewPortTransform(CLIPPED_COORDS[i + k]);
				CLIPPED_COORDS[i + k].refactorPtrs();
			}
			// BACKCULLING
			bool cull = (BACKFACECULLING == true ? ASCIIgLEngine::BackFaceCull(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], CCW) : true);
			Logger::Debug(std::string("[DEBUG] Backface culling result: ") + (cull ? "draw" : "skip"));
			if (cull)
			{
				// DRAWING
				if (WIREFRAME == true || tex == nullptr) {
					Logger::Debug("[DEBUG] Drawing wireframe triangle.");
					DrawTriangleWireFrame(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], PIXEL_SOLID, FG_WHITE);
				}
				else {
					Logger::Debug("[DEBUG] Drawing textured triangle.");
					DrawTriangleTextured(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], tex);
				}
			}
		}
	}
};