#include <thread>
#include <chrono>
#include <algorithm>

#include "Screen.hpp"

#include <engine/Logger.hpp>

int Screen::InitializeScreen(
    const unsigned int width, 
    const unsigned int height, 
    const std::wstring title, 
    const unsigned int fontX, 
    const unsigned int fontY, 
    const unsigned int fpsCap, 
    const float fpsWindowSec, 
    const unsigned short backgroundCol
) {
    Logger::Debug(L"CPU has max" + std::to_wstring(coreCount) + L" threads.");

    Logger::Debug(L"Setting _fpsCap= " + std::to_wstring(fpsCap) + L" and fpsWindow=" + std::to_wstring(fpsWindowSec));
    _fpsCap = fpsCap;
    _fpsWindowSec = fpsWindowSec;

    Logger::Debug(L"Initializing screen with width=" + std::to_wstring(width) + L", height=" + std::to_wstring(height) + L", title=" + title);
    dwBufferSize = COORD{ (SHORT)width, (SHORT)height };
    dwBufferCoord = COORD{ 0, 0 };
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    SCR_TITLE = title;
    rcRegion = SMALL_RECT{ 0, 0, SHORT(width - 1), SHORT(height - 1) };

    TILE_COUNT_X = (SCR_WIDTH + TILE_SIZE_X - 1) / TILE_SIZE_X; // ceiling division
    TILE_COUNT_Y = (SCR_HEIGHT + TILE_SIZE_Y - 1) / TILE_SIZE_Y; // ceiling division

    Logger::Debug(L"Setting font size to " + std::to_wstring(fontX) + L"x" + std::to_wstring(fontY));
    _fontH = fontY;
    _fontW = fontX;

    Logger::Debug(L"Creating front and back console screen buffers for double buffering.");
    _hOutput = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    if (_hOutput == INVALID_HANDLE_VALUE) {
        Logger::Error(L"Failed to create console screen buffers.");
        return SCREEN_WIN_BUFFER_CREATION_FAILED;
    }

    Logger::Debug(L"Setting buffer size.");
    SetConsoleScreenBufferSize(_hOutput, dwBufferSize);

    Logger::Debug(L"Setting console window info to minimal size.");
    SMALL_RECT m_rectWindow = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(_hOutput, TRUE, &m_rectWindow);

	Logger::Debug(L"Creating console font.");
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = _fontW;
    cfi.dwFontSize.Y = _fontH;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, LF_FACESIZE, L"Lucida Console");
    if (!SetCurrentConsoleFontEx(_hOutput, false, &cfi))
        Logger::Error(L"Failed to set font for _hOutput.");

    Logger::Debug(L"Setting physical size of console window.");
    SetConsoleWindowInfo(_hOutput, TRUE, &rcRegion);

    Logger::Debug(L"Setting _hOutput as the active console screen buffer.");
    SetConsoleActiveScreenBuffer(_hOutput);

    Logger::Debug(L"Setting console cursor to invisible.");
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(_hOutput, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(_hOutput, &cursorInfo);

    Logger::Debug(L"Disabling window resizing");
    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
        LONG style = GetWindowLong(hwnd, GWL_STYLE);
        style &= ~WS_MAXIMIZEBOX;
        style &= ~WS_SIZEBOX;
        SetWindowLong(hwnd, GWL_STYLE, style);
    } else {
        Logger::Error(L"Failed to get console window handle for disabling resizing.");
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(_hOutput, &csbi);

    Logger::Debug(L"Maximum window size: " +
                std::to_wstring(csbi.dwMaximumWindowSize.X) + L"x" +
                std::to_wstring(csbi.dwMaximumWindowSize.Y));
    if (SCR_HEIGHT > csbi.dwMaximumWindowSize.Y)
    {
        Logger::Error(L"Requested height exceeds maximum window size.");
        return SCREEN_WIN_HEIGHT_TOO_BIG;
    }
    if (SCR_WIDTH > csbi.dwMaximumWindowSize.X)
    {
        Logger::Error(L"Requested width exceeds maximum window size.");
        return SCREEN_WIN_WIDTH_TOO_BIG;
    }

    Logger::Debug(L"Deleting old buffers and creating new ones.");
	if (pixelBuffer) { delete[] pixelBuffer; pixelBuffer = nullptr; }
	if (depthBuffer) { delete[] depthBuffer; depthBuffer = nullptr; }
	pixelBuffer = new CHAR_INFO[width * height];
	depthBuffer = new float[width * height];

    Logger::Debug(L"Clearing buffers for first draw.");
    _backgroundCol = backgroundCol;
    ClearBuffer();

    Logger::Debug(L"Setting console title.");
    RenderTitle(true);

    Logger::Debug(L"Screen initialization complete.");
    return SCREEN_NOERROR;
}

void Screen::RenderTitle(bool showFps) {
    char titleBuffer[256];

    if (showFps) {
        sprintf_s(
            titleBuffer, sizeof(titleBuffer),
            "ASCIIGL - Console Game Engine - %ls - FPS: %.2f",
            SCR_TITLE.c_str(), std::min(_fps, static_cast<double>(_fpsCap))
        );
    } else {
        sprintf_s(
            titleBuffer, sizeof(titleBuffer),
            "ASCIIGL - Console Game Engine - %ls",
            SCR_TITLE.c_str()
        );
    }

    SetConsoleTitleA(titleBuffer);
}

void Screen::ClearBuffer() {
	// clears the buffer by setting the entire buffer to spaces (ascii code 32)
	std::fill(pixelBuffer, pixelBuffer + SCR_WIDTH * SCR_HEIGHT, CHAR_INFO{' ', _backgroundCol });
	std::fill(depthBuffer, depthBuffer + SCR_WIDTH * SCR_HEIGHT, 0.0f);
}

void Screen::OutputBuffer() {
    // outputs the buffer to the back buffer, then swaps buffers for double buffering
    WriteConsoleOutput(_hOutput, pixelBuffer, dwBufferSize, dwBufferCoord, &rcRegion);
}

void Screen::PlotPixel(glm::vec2 p, CHAR character, short Colour) {
    int x = static_cast<int>(p.x);
    int y = static_cast<int>(p.y);
    if (x >= 0 && x < SCR_WIDTH && y >= 0 && y < SCR_HEIGHT) {
        pixelBuffer[y * SCR_WIDTH + x].Char.AsciiChar = character;
        pixelBuffer[y * SCR_WIDTH + x].Attributes = Colour;
    }
}

void Screen::PlotPixel(glm::vec2 p, CHAR_INFO charCol) {
    int x = static_cast<int>(p.x);
    int y = static_cast<int>(p.y);
    if (x >= 0 && x < SCR_WIDTH && y >= 0 && y < SCR_HEIGHT) {
        pixelBuffer[y * SCR_WIDTH + x] = charCol;
    }
}

void Screen::PlotPixel(int x, int y, CHAR character, short Colour) {
    if (x >= 0 && x < SCR_WIDTH && y >= 0 && y < SCR_HEIGHT) {
        pixelBuffer[y * SCR_WIDTH + x].Char.AsciiChar = character;
        pixelBuffer[y * SCR_WIDTH + x].Attributes = Colour;
    }
}

void Screen::PlotPixel(int x, int y, CHAR_INFO charCol) {
    if (x >= 0 && x < SCR_WIDTH && y >= 0 && y < SCR_HEIGHT) {
        pixelBuffer[y * SCR_WIDTH + x] = charCol;
    }
}

float Screen::GetDeltaTime() {
	return _deltaTime;
}
    
std::wstring Screen::GetTitle() {
    return SCR_TITLE;
}
void Screen::SetTitle(const std::wstring& title) {
    SCR_TITLE = title;
}

unsigned int Screen::GetFontWidth() {
    return _fontW;
}

unsigned int Screen::GetFontHeight() {
    return _fontH;
}

unsigned int Screen::GetWidth() {
    return SCR_WIDTH;
}

unsigned int Screen::GetHeight() {
    return SCR_HEIGHT;
}

void Screen::FPSSampleCalculate(const double currentDeltaTime) {
    Logger::Debug("FPSSampleCalculate called. Current _deltaTime: " + std::to_string(currentDeltaTime));
    _frameTimes.push_back(currentDeltaTime);
    _currDeltaSum += currentDeltaTime;
    Logger::Debug("_frameTimes size after push: " + std::to_string(_frameTimes.size()));

    while (!_frameTimes.empty() && _currDeltaSum > _fpsWindowSec) {
        const double popped_front = _frameTimes.front();
        _frameTimes.pop_front();
        _currDeltaSum -= popped_front;
        Logger::Debug("_frameTimes popped. New size: " + std::to_string(_frameTimes.size()));
    }

    double calculatedFps = _frameTimes.size() * (1 / _currDeltaSum);
    Logger::Debug("Calculated FPS: " + std::to_string(calculatedFps));
    _fps = calculatedFps;
}

void Screen::StartFPSSample()
{
    startTimeFps = std::chrono::system_clock::now();
}

void Screen::EndFPSSample()
{
    endTimeFps = std::chrono::system_clock::now();
    std::chrono::duration<float> deltaTimeTemp = endTimeFps - startTimeFps;
    _deltaTime = deltaTimeTemp.count();
    Logger::Debug("Frame _deltaTime: " + std::to_string(_deltaTime));
}

void Screen::CapFPS() {
    Logger::Debug("CapFPS called. _deltaTime: " + std::to_string(_deltaTime) + ", _fpsCap: " + std::to_string(_fpsCap));
    const float inverseFrameCap = (1.0f / _fpsCap);

    if (_deltaTime < inverseFrameCap) {
        Logger::Debug("Sleeping for " + std::to_string(inverseFrameCap - _deltaTime) + " seconds to cap FPS.");
        std::this_thread::sleep_for(std::chrono::duration<double>(inverseFrameCap - _deltaTime));
        _deltaTime = inverseFrameCap; // Ensure _deltaTime is at least the frame cap
    }
}

void Screen::StartFPSClock() {
    GetInstance().StartFPSSample();
}

void Screen::EndFPSClock() {
    GetInstance().EndFPSSample();
    GetInstance().FPSSampleCalculate(Screen::GetInstance().GetDeltaTime());
    GetInstance().CapFPS();
}

void Screen::SetBackgroundColor(unsigned short color) {
    _backgroundCol = color;
}

unsigned short Screen::GetBackgroundColor() {
    return _backgroundCol;
}

unsigned int Screen::GetTileCountX() {
    return TILE_COUNT_X;
}

unsigned int Screen::GetTileCountY() {
    return TILE_COUNT_Y;
}