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

void Screen::DrawLine(int x1, int y1, int x2, int y2)
{
	std::vector<glm::vec2> line = DrawLineBrensenham(x1, y1, x2, y2);

	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i].x >= 0 && line[i].x < SCR_WIDTH && line[i].y >= 0 && line[i].y < SCR_HEIGHT)
		{
			PlotPixel(line[i]);
		}
	}
}

std::vector<float> Screen::clipToScreen(std::vector<float> p)
{
	std::vector<float> newVert;
	glm::vec4 newPos = glm::vec4(((p[0] + 1.0f) / 2.0f) * SCR_WIDTH, ((p[1] + 1.0f) / 2.0f) * SCR_HEIGHT, p[2], p[3]);
	vec4ToVert(newPos, &newVert);

	if (p.size() > 3)
	{
		for (size_t i = 3; i < p.size(); i++)
		{
			newVert.push_back(p[i]);
		}
	}
	

	return newVert;
}

void Screen::vec4ToVert(glm::vec4 p, std::vector<float>* vert)
{
	vert->push_back(p.x);
	vert->push_back(p.y);
	vert->push_back(p.z);
	vert->push_back(p.w);
}

void Screen::vec3ToVert(glm::vec3 p, std::vector<float>* vert)
{
	vert->push_back(p.x);
	vert->push_back(p.y);
	vert->push_back(p.z);
}

void Screen::vec2ToVert(glm::vec2 p, std::vector<float>* vert)
{
	vert->push_back(p.x);
	vert->push_back(p.y);
}

glm::vec3 Screen::calcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 normal;

	glm::vec3 U = p2 - p1;
	glm::vec3 V = p3 - p1;

	normal = glm::cross(V, U);
	return glm::normalize(normal);
}