#include "Screen.hpp"
#include "LineAlgo.hpp"


Screen::Screen(int width, int height)
	: dwBufferSize(COORD{ (SHORT) width, (SHORT) height }),  dwBufferCoord(COORD{ 0, 0 }), 
	 rcRegion(SMALL_RECT{ 0, 0, SHORT(width - 1), SHORT(height - 1) }), SCR_WIDTH(width), SCR_HEIGHT(height)
{
	// the pixel buffer that is used to output to the screen is a 1d array (cus 2d arrays are cancer and I hate them)
	// it uses the char_info thing since thats what windows uses to input

	pixelBuffer = new CHAR_INFO[width * height];

	ReadConsoleOutput(hOutput, pixelBuffer, dwBufferSize, dwBufferCoord, &rcRegion); // this puts a image of the console in the buffer 
	// (clears garbage memory from other programs)
}

Screen::~Screen()
{

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
		pixelBuffer[i].Char.AsciiChar = (CHAR) 32;
	}
}

void Screen::OutputBuffer()
{
	// outputs the buffer to the console the fastest way
	WriteConsoleOutput(hOutput, pixelBuffer, dwBufferSize, dwBufferCoord, &rcRegion);
}

void Screen::PlotPixel(glm::vec2 p)
{
	// this function takes in a 2d point and plots it on the pixelBuffer
	pixelBuffer[int(p.y) * SCR_WIDTH + int(p.x)].Char.AsciiChar = (CHAR) 35; // setting the point in the screen buffer to a hashtag 
	// also reversing the x and y else the image would be flipped (35 = ascii code for #)
}

template<typename T>
void Screen::RenderTriangles(T& VERTEX_SHADER, std::vector<float*> vertices)
{
	std::vector<float*> transformed;

	for (int i = -1; i < vertices.size() - 1; i++)
	{
		// transforming vertices using vertex shader
		glm::vec4 newVert = VERTEX_SHADER.Use(vertices[i]);
		transformed.push_back({ newVert[0], newVert[1], newVert[2] });

		// perspective division
		transformed[-1][0] /= transformed[-1][3];
		transformed[-1][1] /= transformed[-1][3];
		transformed[-1][2] /= transformed[-1][3];
	}

	for (size_t = 0; i < transformed.size(); i++)
	{
		DrawLine(transformed[i][0], transformed[i][1], transformed[i + 1][0], transformed[i + 1][1]);
	}

	DrawLine(0, 0, SCR_WIDTH - 1, 0);
	DrawLine(SCR_WIDTH - 1, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1);
	DrawLine(SCR_WIDTH - 1, SCR_HEIGHT - 1, 0, SCR_HEIGHT - 1);
	DrawLine(0, SCR_HEIGHT - 1, 0, 0);
}

void Screen::DrawLine(int x1, int y1, int x2, int y2)
{
	std::vector<glm::vec2> line = DrawLineBrensenham(x1, y1, x2, y2);

	for (size_t i = 0; i < line.size(); i++)
	{
		PlotPixel(line[i]);
	}
}