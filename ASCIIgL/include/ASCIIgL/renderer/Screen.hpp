#pragma once

// External libraries
#include "../../vendor/glm/glm.hpp"
#include "../../vendor/glm/gtc/matrix_transform.hpp"

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
#include <thread>

// Engine includes
#include "../engine/Logger.hpp"
#include "../renderer/RenderEnums.hpp"

// Error codes
enum ScreenError {
    SCREEN_NOERROR = 0,
    SCREEN_WIN_BUFFER_CREATION_FAILED = -1,
    SCREEN_WIN_HEIGHT_TOO_BIG = -2,
    SCREEN_WIN_WIDTH_TOO_BIG = -3
};

class Screen {
private:
    // parallel processing
    static inline unsigned int coreCount = std::thread::hardware_concurrency();

    static inline unsigned int TILE_COUNT_X = 0;
    static inline unsigned int TILE_COUNT_Y = 0;

    // Console handles and buffers
    static inline HANDLE _hOutput = nullptr;
    static inline COORD dwBufferSize = {0, 0};
    static inline COORD dwBufferCoord = {0, 0};
    static inline SMALL_RECT rcRegion = {0, 0, 0, 0};

    // Singleton instance
	Screen() = default;
	Screen(const Screen&) = delete;
	Screen& operator=(const Screen&) = delete;

    // Timing and FPS
    static inline std::chrono::system_clock::time_point startTimeFps = std::chrono::system_clock::now();
    static inline std::chrono::system_clock::time_point endTimeFps = std::chrono::system_clock::now();
    static inline double _fpsWindowSec = 1.0f;
    static inline double _fps = 0.0f;
	static inline double _currDeltaSum = 0.0f;
    static inline double _deltaTime = 0.0f;
    static inline std::deque<double> _frameTimes = {};
    static inline unsigned int _fpsCap = 60;

    // Screen properties
    static inline int SCR_WIDTH = 0;
    static inline int SCR_HEIGHT = 0;
    static inline unsigned int _fontW = 0;
    static inline unsigned int _fontH = 0;
    static inline std::wstring SCR_TITLE = L"";
    static inline unsigned short _backgroundCol = BG_BLACK;

    // Internal FPS helpers
    static void StartFPSSample();
    static void EndFPSSample();
    static void CapFPS();
    static void FPSSampleCalculate(const double currentDeltaTime);

public:
    static Screen& GetInstance() {
		static Screen instance;
		return instance;
	}

    // Construction
    static int InitializeScreen(
        const unsigned int width, 
        const unsigned int height, 
        const std::wstring title, 
        const unsigned int fontX, 
        const unsigned int fontY, 
        const unsigned int _fpsCap, 
        const float _fpsWindowSec, 
        const unsigned short _backgroundCol
    );

    // Rendering and buffer management
    static void StartFPSClock();
    static void EndFPSClock();
    static void RenderTitle(bool showFps);
    static void ClearBuffer();
    static void OutputBuffer();
    static void PlotPixel(glm::vec2 p, CHAR character, short Colour);
    static void PlotPixel(glm::vec2 p, CHAR_INFO charCol);
    static void PlotPixel(int x, int y, CHAR character, short Colour);
    static void PlotPixel(int x, int y, CHAR_INFO charCol);

    // Getters and Setters
    static float GetDeltaTime();

    static std::wstring GetTitle();
    static void SetTitle(const std::wstring& title);

    static unsigned int GetFontWidth();
    static unsigned int GetFontHeight();
    static unsigned int GetWidth();
    static unsigned int GetHeight();
    static unsigned int GetTileCountX();
    static unsigned int GetTileCountY();

    static unsigned short GetBackgroundColor();
    static void SetBackgroundColor(const unsigned short color);

    static inline CHAR_INFO* pixelBuffer = nullptr;
    static inline float* depthBuffer = nullptr;

    static inline const unsigned int TILE_SIZE_X = 16;
    static inline const unsigned int TILE_SIZE_Y = 16;
};