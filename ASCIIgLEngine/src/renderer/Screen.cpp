#include <thread>
#include <chrono>
#include <algorithm>

#include "Screen.hpp"

#include <engine/Logger.hpp>

int Screen::InitializeScreen(unsigned int width, unsigned int height, const std::wstring title, unsigned int fontX, unsigned int fontY)
{
    Logger::Debug(L"[DEBUG] Initializing screen with width=" + std::to_wstring(width) +
                L", height=" + std::to_wstring(height) + L", title=" + title);
    dwBufferSize = COORD{ (SHORT)width, (SHORT)height };
    dwBufferCoord = COORD{ 0, 0 };
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    SCR_TITLE = title;
    fontH = fontY;
    fontW = fontX;
    rcRegion = SMALL_RECT{ 0, 0, SHORT(width - 1), SHORT(height - 1) };

    Logger::Debug(L"[DEBUG] Setting console window info to minimal size.");
    SMALL_RECT m_rectWindow = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(hOutput, TRUE, &m_rectWindow);

    Logger::Debug(L"[DEBUG] Assigning screen buffer to console.");
    SetConsoleActiveScreenBuffer(hOutput);

    Logger::Debug(L"[DEBUG] Setting buffer size.");
    SetConsoleScreenBufferSize(hOutput, dwBufferSize);

	Logger::Debug(L"[DEBUG] Creating console font.");
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = fontH;
    cfi.dwFontSize.Y = fontW;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"SimSum-ExtB");
    SetCurrentConsoleFontEx(hOutput, false, &cfi);

    Logger::Debug(L"[DEBUG] Setting physical size of console window.");
    SetConsoleWindowInfo(hOutput, TRUE, &rcRegion);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOutput, &csbi);

    Logger::Debug(L"[DEBUG] Maximum window size: " +
                std::to_wstring(csbi.dwMaximumWindowSize.X) + L"x" +
                std::to_wstring(csbi.dwMaximumWindowSize.Y));
    if (SCR_HEIGHT > csbi.dwMaximumWindowSize.Y)
    {
        Logger::Error(L"[ERROR] Requested height exceeds maximum window size.");
        delete Instance;
        return WIN_HEIGHT_TOO_BIG;
    }
    if (SCR_WIDTH > csbi.dwMaximumWindowSize.X)
    {
        Logger::Error(L"[ERROR] Requested width exceeds maximum window size.");
        delete Instance;
        return WIN_WIDTH_TOO_BIG;
    }

    Logger::Debug(L"[DEBUG] Deleting old buffers and creating new ones.");
	if (pixelBuffer) { delete[] pixelBuffer; pixelBuffer = nullptr; }
	if (colourBuffer) { delete[] colourBuffer; colourBuffer = nullptr; }
	if (depthBuffer) { delete[] depthBuffer; depthBuffer = nullptr; }
	pixelBuffer = new CHAR_INFO[width * height];
	colourBuffer = new glm::vec3[width * height];
	depthBuffer = new float[width * height];

    Logger::Debug(L"[DEBUG] Clearing buffers for first draw.");
    ReadConsoleOutput(hOutput, pixelBuffer, dwBufferSize, dwBufferCoord, &rcRegion);
    std::fill(colourBuffer, colourBuffer + SCR_WIDTH * SCR_HEIGHT, glm::vec3(0, 0, 0));
    std::fill(depthBuffer, depthBuffer + SCR_WIDTH * SCR_HEIGHT, 0.0f);

    Logger::Debug(L"[DEBUG] Setting console title.");
    SetTitle(true);

    Logger::Debug(L"[DEBUG] Screen initialization complete.");
    return NOERROR;
}

Screen::Screen() // setting up singleton class, default constructor is needed
{

}

Screen::~Screen() // this deletes stuff I allocated on the heap
{
	delete Instance, pixelBuffer, colourBuffer, depthBuffer;
}

Screen* Screen::GetInstance() // singleton class functionality, gets pointer to itself guaranteeing only one can exist
{
	if (Instance == nullptr)
	{
		Instance = new Screen();
		return Instance;
	}
	else
	{
		return Instance;
	}
}

void Screen::SetTitle(bool showFps) {
    char titleBuffer[256];

    if (showFps) {
        sprintf_s(
            titleBuffer, sizeof(titleBuffer),
            "ASCIIGL - Console Game Engine - %ls - FPS: %.2f",
            SCR_TITLE.c_str(), std::min(fps, static_cast<double>(frameCap))
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

void Screen::ClearScreen()
{
	COORD cursorPosition;
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
	// uses native WIN API commands to scroll clear it
}

void Screen::ClearBuffer(unsigned short backgrounCol)
{
	// clears the buffer by setting the entire buffer to spaces (ascii code 32)
	std::fill(pixelBuffer, pixelBuffer + SCR_WIDTH * SCR_HEIGHT, CHAR_INFO{' ', backgrounCol });
	std::fill(colourBuffer, colourBuffer + SCR_WIDTH * SCR_HEIGHT, glm::vec3(0, 0, 0));
	std::fill(depthBuffer, depthBuffer + SCR_WIDTH * SCR_HEIGHT, 0.0f);
}

void Screen::OutputBuffer()
{
	// outputs the buffer to the console the fastest way
	WriteConsoleOutput(hOutput, pixelBuffer, dwBufferSize, dwBufferCoord, &rcRegion);
}

void Screen::PlotPixel(glm::vec2 p, CHAR character, short Colour)
{
	// this function takes in a 2d point and plots it on the pixelBuffer

	pixelBuffer[int(p.y) * SCR_WIDTH + int(p.x)].Char.AsciiChar = character; // setting the point in the screen buffer to a hashtag 
	pixelBuffer[int(p.y) * SCR_WIDTH + int(p.x)].Attributes = Colour;

	// also reversing the x and y else the image would be flipped (35 = ascii code for #)
}

void Screen::PlotPixel(glm::vec2 p, CHAR_INFO charCol)
{
	pixelBuffer[int(p.y) * SCR_WIDTH + int(p.x)] = charCol; // plots pixel using a char_info instead of individual attributes
}

void Screen::DrawBorder(short col)
{
	// DRAWING BORDERS
	DrawLine(1, 1, SCR_WIDTH - 1, 1, PIXEL_SOLID, col);
	DrawLine(SCR_WIDTH - 1, 1, SCR_WIDTH - 1, SCR_HEIGHT - 1, PIXEL_SOLID, col);
	DrawLine(SCR_WIDTH - 1, SCR_HEIGHT - 1, 1, SCR_HEIGHT - 1, PIXEL_SOLID, col);
	DrawLine(1, 1, 1, SCR_HEIGHT - 1, PIXEL_SOLID, col);
}

float Screen::GetDeltaTime()
{
	return deltaTime;
}


void Screen::DrawLine(int x1, int y1, int x2, int y2, CHAR pixel_type, short col)
{
	// Got this algorithm from Code-It-Yourself! 3D Graphics Engine tutorial series by javidx9 on youtube

	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x, y;

	dx = x2 - x1;
	dy = y2 - y1;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = 1;
	if (x2 < x1)
		incx = -1;
	incy = 1;
	if (y2 < y1)
		incy = -1;
	x = x1;
	y = y1;
	if (dx > dy)
	{
		if (x > 0 && x < SCR_WIDTH && y > 0 && y < SCR_HEIGHT)
			PlotPixel(glm::vec2(x, y), pixel_type, col);
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		for (i = 0; i < dx; i++)
		{
			if (e >= 0)
			{
				y += incy;
				if (x > 0 && x < SCR_WIDTH && y > 0 && y < SCR_HEIGHT)
					PlotPixel(glm::vec2(x, y), pixel_type, col);
				e += inc1;
				x += incx;
			}
			else
			{
				e += inc2; x += incx;
				//points.push_back(glm::vec2(x, y - incy));
			}
			if (x > 0 && x < SCR_WIDTH && y > 0 && y < SCR_HEIGHT)
				PlotPixel(glm::vec2(x, y), pixel_type, col);
		}
	}
	else
	{
		if (x > 0 && x < SCR_WIDTH && y > 0 && y < SCR_HEIGHT)
			PlotPixel(glm::vec2(x, y), pixel_type, col);
		e = 2 * dx - dy;
		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;
		for (i = 0; i < dy; i++)
		{
			if (e >= 0)
			{
				x += incx;
				if (x > 0 && x < SCR_WIDTH && y > 0 && y < SCR_HEIGHT)
					PlotPixel(glm::vec2(x, y), pixel_type, col);
				y += incy;
				e += inc1;

			}
			else
			{
				e += inc2; y += incy;
				//points.push_back(glm::vec2(x + incx, y));
			}

			if (x > 0 && x < SCR_WIDTH && y > 0 && y < SCR_HEIGHT)
				PlotPixel(glm::vec2(x, y), pixel_type, col);
		}
	}

	// Shade Characters is #x/- in order of luminescence
}

void Screen::DrawTriangleWireFrame(VERTEX v1, VERTEX v2, VERTEX v3, CHAR pixel_type, short col) {
	// RENDERING LINES BETWEEN VERTICES
	DrawLine((int) *v1.x, (int) *v1.y, (int) *v2.x, (int) *v2.y, pixel_type, col);
	DrawLine((int) *v2.x, (int) *v2.y, (int) *v3.x, (int) *v3.y, pixel_type, col);
	DrawLine((int) *v3.x, (int) *v3.y, (int) *v1.x, (int) *v1.y, pixel_type, col);
}

void Screen::DrawTriangleTextured(VERTEX vert1, VERTEX vert2, VERTEX vert3, Texture* tex) {
    int texWidth = tex->GetWidth();
    int texHeight = tex->GetHeight();
    if (texWidth == 0 || texHeight == 0)
    {
        Logger::Error("[ERROR] Invalid texture size. Width: " + std::to_string(texWidth) + ", Height: " + std::to_string(texHeight));
        return;
    }

    int x1 = *vert1.x, x2 = *vert2.x, x3 = *vert3.x;
    int y1 = *vert1.y, y2 = *vert2.y, y3 = *vert3.y;
    float w1 = *vert1.uvw, w2 = *vert2.uvw, w3 = *vert3.uvw;
    float u1 = *vert1.u, u2 = *vert2.u, u3 = *vert3.u;
    float v1 = *vert1.v, v2 = *vert2.v, v3 = *vert3.v;

    // Sort vertices by y
    if (y2 < y1) { std::swap(y1, y2); std::swap(x1, x2); std::swap(u1, u2); std::swap(v1, v2); std::swap(w1, w2); }
    if (y3 < y1) { std::swap(y1, y3); std::swap(x1, x3); std::swap(u1, u3); std::swap(v1, v3); std::swap(w1, w3); }
    if (y3 < y2) { std::swap(y2, y3); std::swap(x2, x3); std::swap(u2, u3); std::swap(v2, v3); std::swap(w2, w3); }

    // Precompute steps
    int dy1 = y2 - y1, dx1 = x2 - x1;
    int dy2 = y3 - y1, dx2 = x3 - x1;
    float du1 = u2 - u1, dv1 = v2 - v1, dw1 = w2 - w1;
    float du2 = u3 - u1, dv2 = v3 - v1, dw2 = w3 - w1;

    float dax_step = dy1 ? dx1 / (float)abs(dy1) : 0;
    float dbx_step = dy2 ? dx2 / (float)abs(dy2) : 0;
    float du1_step = dy1 ? du1 / (float)abs(dy1) : 0;
    float dv1_step = dy1 ? dv1 / (float)abs(dy1) : 0;
    float dw1_step = dy1 ? dw1 / (float)abs(dy1) : 0;
    float du2_step = dy2 ? du2 / (float)abs(dy2) : 0;
    float dv2_step = dy2 ? dv2 / (float)abs(dy2) : 0;
    float dw2_step = dy2 ? dw2 / (float)abs(dy2) : 0;

    // Upper part
    if (dy1)
    {
        for (int i = y1; i <= y2 && i < SCR_HEIGHT; i++)
        {
            int ax = x1 + (float)(i - y1) * dax_step;
            int bx = x1 + (float)(i - y1) * dbx_step;
            float tex_su = u1 + (float)(i - y1) * du1_step;
            float tex_sv = v1 + (float)(i - y1) * dv1_step;
            float tex_sw = w1 + (float)(i - y1) * dw1_step;
            float tex_eu = u1 + (float)(i - y1) * du2_step;
            float tex_ev = v1 + (float)(i - y1) * dv2_step;
            float tex_ew = w1 + (float)(i - y1) * dw2_step;
            if (ax > bx) { std::swap(ax, bx); std::swap(tex_su, tex_eu); std::swap(tex_sv, tex_ev); std::swap(tex_sw, tex_ew); }
            float tstep = (bx != ax) ? 1.0f / ((float)(bx - ax)) : 0.0f;
            float t = 0.0f;
            for (int j = ax; j < bx && j < SCR_WIDTH; j++)
            {
                float tex_w = (1.0f - t) * tex_sw + t * tex_ew;
                if (tex_w > depthBuffer[i * SCR_WIDTH + j]) {
                    float tex_uw = ((1.0f - t) * tex_su + t * tex_eu) / tex_w;
                    float tex_vw = ((1.0f - t) * tex_sv + t * tex_ev) / tex_w;
                    float texWidthProd = tex_uw * texWidth;
                    float texHeightProd = tex_vw * texHeight;
                    if (tex_uw < 1 && tex_vw < 1) {
                        float blendedGrayScale = ASCIIgLEngine::GrayScaleRGB(tex->GetPixelCol(texWidthProd, texHeightProd));
                        PlotPixel(glm::vec2(j, i), ASCIIgLEngine::GetColGlyph(blendedGrayScale));
                        depthBuffer[i * SCR_WIDTH + j] = tex_w;
                    }
                }
                t += tstep;
            }
        }
    }

    // Lower part
    dy1 = y3 - y2; dx1 = x3 - x2;
    du1 = u3 - u2; dv1 = v3 - v2; dw1 = w3 - w2;
    dax_step = dy1 ? dx1 / (float)abs(dy1) : 0;
    dbx_step = dy2 ? dx2 / (float)abs(dy2) : 0;
    du1_step = dy1 ? du1 / (float)abs(dy1) : 0;
    dv1_step = dy1 ? dv1 / (float)abs(dy1) : 0;
    dw1_step = dy1 ? dw1 / (float)abs(dy1) : 0;
    if (dy1)
    {
        for (int i = y2; i <= y3 && i < SCR_HEIGHT; i++)
        {
            int ax = x2 + (float)(i - y2) * dax_step;
            int bx = x1 + (float)(i - y1) * dbx_step;
            float tex_su = u2 + (float)(i - y2) * du1_step;
            float tex_sv = v2 + (float)(i - y2) * dv1_step;
            float tex_sw = w2 + (float)(i - y2) * dw1_step;
            float tex_eu = u1 + (float)(i - y1) * du2_step;
            float tex_ev = v1 + (float)(i - y1) * dv2_step;
            float tex_ew = w1 + (float)(i - y1) * dw2_step;
            if (ax > bx) { std::swap(ax, bx); std::swap(tex_su, tex_eu); std::swap(tex_sv, tex_ev); std::swap(tex_sw, tex_ew); }
            float tstep = (bx != ax) ? 1.0f / ((float)(bx - ax)) : 0.0f;
            float t = 0.0f;
            for (int j = ax; j < bx && j < SCR_WIDTH; j++)
            {
                float tex_w = (1.0f - t) * tex_sw + t * tex_ew;
                if (tex_w > depthBuffer[i * SCR_WIDTH + j])
                {
                    float tex_uw = ((1.0f - t) * tex_su + t * tex_eu) / tex_w;
                    float tex_vw = ((1.0f - t) * tex_sv + t * tex_ev) / tex_w;
                    if (tex_uw < 1 && tex_vw < 1)
                    {
                        float blendedGrayScale = ASCIIgLEngine::GrayScaleRGB(tex->GetPixelCol(tex_uw * texWidth, tex_vw * texHeight));
                        PlotPixel(glm::vec2(j, i), ASCIIgLEngine::GetColGlyph(blendedGrayScale));
                        depthBuffer[i * SCR_WIDTH + j] = tex_w;
                    }
                }
                t += tstep;
            }
        }
    }
}

void Screen::FPSSampleCalculate(double currentDeltaTime) {
    Logger::Debug("[DEBUG] FPSSampleCalculate called. Current deltaTime: " + std::to_string(currentDeltaTime));
    frameTimes.push_back(currentDeltaTime);
    currDeltaSum += currentDeltaTime;
    Logger::Debug("[DEBUG] frameTimes size after push: " + std::to_string(frameTimes.size()));

    while (!frameTimes.empty() && currDeltaSum > fpsWindowSec) {
        const double popped_front = frameTimes.front();
        frameTimes.pop_front();
        currDeltaSum -= popped_front;
        Logger::Debug("[DEBUG] frameTimes popped. New size: " + std::to_string(frameTimes.size()));
    }

    double calculatedFps = frameTimes.size() * (1 / fpsWindowSec);
    Logger::Debug("[DEBUG] Calculated FPS: " + std::to_string(calculatedFps));
    fps = calculatedFps;
}

void Screen::StartFPSSample()
{
    startTimeFps = std::chrono::system_clock::now();
}

void Screen::EndFPSSample()
{
    endTimeFps = std::chrono::system_clock::now();
    std::chrono::duration<float> deltaTimeTemp = endTimeFps - startTimeFps;
    deltaTime = deltaTimeTemp.count();
    Logger::Debug("[DEBUG] Frame deltaTime: " + std::to_string(deltaTime));
}

void Screen::CapFPS() {
    Logger::Debug("[DEBUG] CapFPS called. deltaTime: " + std::to_string(deltaTime) + ", frameCap: " + std::to_string(frameCap));
    const float inverseFrameCap = (1.0f / frameCap);

    if (deltaTime < inverseFrameCap) {
        Logger::Debug("[DEBUG] Sleeping for " + std::to_string(inverseFrameCap - deltaTime) + " seconds to cap FPS.");
        std::this_thread::sleep_for(std::chrono::duration<double>(inverseFrameCap - deltaTime));

        deltaTime = inverseFrameCap; // Ensure deltaTime is at least the frame cap
        fps = frameCap;
    }
}

void Screen::StartFPSClock() {
    GetInstance()->StartFPSSample();
}

void Screen::EndFPSClock() {
    GetInstance()->EndFPSSample();
    GetInstance()->FPSSampleCalculate(Screen::GetInstance()->GetDeltaTime());
    GetInstance()->CapFPS();
}

VERTEX Screen::ViewPortTransform(VERTEX vertice) {
	// transforms vertice from [-1 to 1] to [0 to scr_dim]
	VERTEX newVert = vertice;
	glm::vec4 newPos = glm::vec4(((*vertice.x + 1.0f) / 2.0f) * SCR_WIDTH, ((*vertice.y + 1.0f) / 2.0f) * SCR_HEIGHT, *vertice.z, *vertice.w);
	newVert.SetXYZW(newPos);

	return newVert;
}

void Screen::RenderTriangles(VERTEX_SHADER VSHADER, std::vector<VERTEX> vertices, Texture* tex) {
    if (vertices.size() < 3 || vertices.size() % 3 != 0) {
        return;
    }

    for (int k = 0; k < vertices.size(); k++) 
    { 
        VSHADER.GLUse(vertices[k]); 
    }

    std::vector<VERTEX> CLIPPED_COORDS;
    ASCIIgLEngine::ClippingHelper(vertices, CLIPPED_COORDS);
    for (int i = 0; i < CLIPPED_COORDS.size(); i += 3)
    {
        for (int k = 0; k < 3; k++)
        {
            ASCIIgLEngine::PerspectiveDivision(CLIPPED_COORDS, i + k);
            CLIPPED_COORDS[i + k] = ViewPortTransform(CLIPPED_COORDS[i + k]);
            CLIPPED_COORDS[i + k].refactorPtrs();
        }
        bool cull = (BACKFACECULLING == true ? ASCIIgLEngine::BackFaceCull(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], CCW) : true);
        if (cull)
        {
            if (WIREFRAME == true || tex == nullptr) {
                DrawTriangleWireFrame(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], PIXEL_SOLID, FG_WHITE);
            }
            else {
                DrawTriangleTextured(CLIPPED_COORDS[i], CLIPPED_COORDS[i + 1], CLIPPED_COORDS[i + 2], tex);
            }
        }
    }
}