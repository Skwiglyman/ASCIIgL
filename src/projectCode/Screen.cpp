#include "Screen.hpp"

void Screen::InitializeScreen(int width, int height, std::wstring title)
	
{
	// innitializing all of the variables
	dwBufferSize = COORD{ (SHORT)width, (SHORT)height };
	dwBufferCoord = COORD{ 0, 0 };
	rcRegion = SMALL_RECT{ 0, 0, SHORT(width - 1), SHORT(height - 1) };
	SCR_WIDTH = width; 
	SCR_HEIGHT = height;
	SCR_TITLE = title;

	// the pixel buffer that is used to output to the screen is a 1d array (cus 2d arrays are cancer and I hate them)
	// it uses the char_info thing since thats what windows uses to input

	delete pixelBuffer;
	pixelBuffer = new CHAR_INFO[width * height];
	ReadConsoleOutput(hOutput, pixelBuffer, dwBufferSize, dwBufferCoord, &rcRegion); // this puts a image of the console in the buffer 
	// (clears garbage memory from other programs)
}

Screen::Screen()
{

}

Screen::~Screen()
{

}

Screen* Screen::GetInstance()
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

void Screen::SetTitle()
{
	wchar_t s[256];
	swprintf_s(s, 256, L"ASCIIGL - Console Game Engine - %s - FPS: %3.2f", SCR_TITLE.c_str(), 1.0f / fElapsedTime);
	SetConsoleTitle(s);
}

void Screen::ClearScreen()
{
	// I think this function scrolls the console past the outputted text and sort of pseudo clears the console (I just ctrl c ctrl v off cus fuck learning windows api)

	COORD cursorPosition;
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
	// uses native WIN API commands to clear it
}

void Screen::ClearBuffer()
{
	// clears the buffer by setting the entire buffer to spaces (ascii code 32)
	for (int i = 0; i < SCR_WIDTH * SCR_HEIGHT; i++)
	{
		pixelBuffer[i].Char.AsciiChar = ' ';
		pixelBuffer[i].Attributes = 0x0000;
	}
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


void Screen::DrawLine(int x1, int y1, int x2, int y2, CHAR pixel_type, short col)
{
	std::vector<glm::vec2> line = DrawLineBrensenham(x1, y1, x2, y2);

	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i].x >= 0 && line[i].x < SCR_WIDTH && line[i].y >= 0 && line[i].y < SCR_HEIGHT)
		{
			PlotPixel(line[i], pixel_type, col);
		}
	}
	// Shade Characters is #x/- in order of luminescence
}

void Screen::DrawTriangleFill(VERTEX v1, VERTEX v2, VERTEX v3)
{
	std::vector<glm::vec2> triangle = FillTriangle((int) *v1.x, (int) *v1.y, (int) *v2.x, (int) *v2.y, (int) *v3.x, (int) *v3.y);

	for (size_t i = 0; i < triangle.size(); i++)
	{
		if (triangle[i].x >= 0 && triangle[i].x < SCR_WIDTH && triangle[i].y >= 0 && triangle[i].y < SCR_HEIGHT)
		{
			PlotPixel(triangle[i], PIXEL_SOLID, FG_WHITE);
		}
	}
	// Shade Characters is #x/- in order of luminescence
}

void Screen::DrawTriangleWireFrame(VERTEX v1, VERTEX v2, VERTEX v3)
{
	// RENDERING LINES BETWEEN VERTICES
	DrawLine((int) *v1.x, (int) *v1.y, (int) *v2.x, (int) *v2.y, PIXEL_SOLID, FG_WHITE);
	DrawLine((int) *v2.x, (int) *v2.y, (int) *v3.x, (int) *v3.y, PIXEL_SOLID, FG_WHITE);
	DrawLine((int) *v3.x, (int) *v3.y, (int) *v1.x, (int) *v1.y, PIXEL_SOLID, FG_WHITE);
}

VERTEX Screen::ViewPortTransform(VERTEX vertice)
{
	VERTEX newVert = vertice;
	glm::vec4 newPos = glm::vec4(((*vertice.x + 1.0f) / 2.0f) * SCR_WIDTH, ((*vertice.y + 1.0f) / 2.0f) * SCR_HEIGHT, *vertice.z, *vertice.w);
	newVert.SetXYZW(newPos);

	return newVert;
}