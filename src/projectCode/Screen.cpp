#include "Screen.hpp"
#include <algorithm>


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