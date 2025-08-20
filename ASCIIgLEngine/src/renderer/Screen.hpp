#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>

#include "VertexShader.hpp"
#include "Vertex.hpp"
#include "ASCIIgLEngine.hpp"	
#include "Texture.hpp"
#include "engine/Logger.hpp"

#ifndef NOERROR
#define NOERROR 1
#endif
#define WIN_WIDTH_TOO_BIG 2
#define WIN_HEIGHT_TOO_BIG 3

#include <deque>

class Screen {
	private:
		HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);

		COORD dwBufferSize;
		COORD dwBufferCoord;
		SMALL_RECT rcRegion;
		
		Screen();
		static inline Screen* Instance = nullptr;

		inline static std::chrono::system_clock::time_point startTimeFps = std::chrono::system_clock::now();
		inline static std::chrono::system_clock::time_point endTimeFps = std::chrono::system_clock::now();
		inline static double fpsWindowSec = 0.5f;
		inline static double fps = 0.0f;
		inline static double deltaTime = 0.0f;
		inline static double currDeltaSum = 0.0f;
		inline static std::deque<double> frameTimes = {};
		inline static unsigned int frameCap = 60;

		void DrawLine(int x1, int y1, int x2, int y2, CHAR pixel_type, short col);
		void DrawTriangleWireFrame(VERTEX v1, VERTEX v2, VERTEX v3, CHAR pixel_type, short col);
		void DrawTriangleTextured(VERTEX vert1, VERTEX vert2, VERTEX vert3, Texture* tex);

		void StartFPSSample();
		void EndFPSSample();
		void CapFPS();
		void FPSSampleCalculate(double currentDeltaTime);

	public:
		CHAR_INFO* pixelBuffer = nullptr;
		glm::vec3* colourBuffer = nullptr;
		float* depthBuffer = nullptr;
		
		int SCR_WIDTH;
		int SCR_HEIGHT; 
		std::wstring SCR_TITLE;
		
		
		unsigned int fontW;
		unsigned int fontH;
		
		static Screen* GetInstance();
		int InitializeScreen(unsigned int width, unsigned int height, const std::wstring title, unsigned int fontX, unsigned int fontY);

		Screen(const Screen& obj) = delete;
		Screen(Screen&& obj) = delete;
		~Screen();
		
		VERTEX ViewPortTransform(VERTEX vertice);
		
		void StartFPSClock();
		void EndFPSClock();
		
		void SetTitle(bool showFps);
		void ClearScreen();
		void ClearBuffer(unsigned short backgrounCol);
		void OutputBuffer();
		void PlotPixel(glm::vec2 p, CHAR character, short Colour);
		void PlotPixel(glm::vec2 p, CHAR_INFO charCol);
		void DrawBorder(short col);

		float GetDeltaTime(); 

		bool WIREFRAME = true;
		bool BACKFACECULLING = true;
		bool CCW = true;

		void RenderTriangles(VERTEX_SHADER VSHADER, std::vector<VERTEX> vertices, Texture* tex);
};