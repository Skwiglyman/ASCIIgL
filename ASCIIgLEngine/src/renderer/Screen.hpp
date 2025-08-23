#pragma once

// External libraries
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

// Windows API
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

// STL
#include <deque>
#include <chrono>

// Engine includes
#include "engine/Logger.hpp"

// Error codes
#ifndef NOERROR
#define NOERROR 1
#endif
#define WIN_WIDTH_TOO_BIG 2
#define WIN_HEIGHT_TOO_BIG 3

class Screen {
private:
    // Console handles and buffers
    inline static HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);
    static inline COORD dwBufferSize = {0, 0};
    static inline COORD dwBufferCoord = {0, 0};
    static inline SMALL_RECT rcRegion = {0, 0, 0, 0};

    // Singleton instance
	Screen() = default;
	Screen(const Screen&) = delete;
	Screen& operator=(const Screen&) = delete;

    // Timing and FPS
    inline static std::chrono::system_clock::time_point startTimeFps = std::chrono::system_clock::now();
    inline static std::chrono::system_clock::time_point endTimeFps = std::chrono::system_clock::now();
    inline static double fpsWindowSec = 1.0f;
    inline static double fps = 0.0f;
	inline static double currDeltaSum = 0.0f;
    inline static double deltaTime = 0.0f;
    inline static std::deque<double> frameTimes = {};
    inline static unsigned int fpsCap = 60;

    // Screen properties
    static inline int SCR_WIDTH = 0;
    static inline int SCR_HEIGHT = 0;
    static inline unsigned int fontW = 0;
    static inline unsigned int fontH = 0;
    static inline std::wstring SCR_TITLE = L"";

    // Internal FPS helpers
    static void StartFPSSample();
    static void EndFPSSample();
    static void CapFPS();
    static void FPSSampleCalculate(double currentDeltaTime);

public:
    static Screen& GetInstance() {
		static Screen instance;
		return instance;
	}

    // Construction
    int InitializeScreen(unsigned int width, unsigned int height, const std::wstring title, unsigned int fontX, unsigned int fontY, unsigned int fpsCap, float fpsWindowSec);

    // Rendering and buffer management
    static void StartFPSClock();
    static void EndFPSClock();
    static void RenderTitle(bool showFps);
    static void ClearScreen();
    static void ClearBuffer(unsigned short backgrounCol);
    static void OutputBuffer();
    static void PlotPixel(glm::vec2 p, CHAR character, short Colour);
    static void PlotPixel(glm::vec2 p, CHAR_INFO charCol);
    static void PlotPixel(int x, int y, CHAR character, short Colour);
    static void PlotPixel(int x, int y, CHAR_INFO charCol);

    // Getters
    static float GetDeltaTime();
    static std::wstring GetTitle();
    static void SetTitle(const std::wstring& title);
    static unsigned int GetFontWidth();
    static unsigned int GetFontHeight();
    static unsigned int GetWidth();
    static unsigned int GetHeight();

    static inline CHAR_INFO* pixelBuffer = nullptr;
    static inline glm::vec3* colourBuffer = nullptr;
    static inline float* depthBuffer = nullptr;
};