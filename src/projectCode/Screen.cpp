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

	SetTitle();
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
	std::chrono::duration<float> elapsedTimeTemp = endTimeFps - startTimeFps;
	elapsedTime = elapsedTimeTemp.count();

	wchar_t s[256];
	swprintf_s(s, 256, L"ASCIIGL - Console Game Engine - %s - FPS: %3.2f", SCR_TITLE.c_str(), 1.0f / (elapsedTime > 0 ? elapsedTime : 0.0001));
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
		PlotPixel(glm::vec2(x, y), pixel_type, col);
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		for (i = 0; i < dx; i++)
		{
			if (e >= 0)
			{
				y += incy;
				PlotPixel(glm::vec2(x, y), pixel_type, col);
				e += inc1;
				x += incx;
			}
			else
			{
				e += inc2; x += incx;
				//points.push_back(glm::vec2(x, y - incy));
			}
			PlotPixel(glm::vec2(x, y), pixel_type, col);
		}
	}
	else
	{
		PlotPixel(glm::vec2(x, y), pixel_type, col);
		e = 2 * dx - dy;
		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;
		for (i = 0; i < dy; i++)
		{
			if (e >= 0)
			{
				x += incx;
				PlotPixel(glm::vec2(x, y), pixel_type, col);
				y += incy;
				e += inc1;

			}
			else
			{
				e += inc2; y += incy;
				//points.push_back(glm::vec2(x + incx, y));
			}

			PlotPixel(glm::vec2(x, y), pixel_type, col);
		}
	}

	// Shade Characters is #x/- in order of luminescence
}

void Screen::DrawTriangleFill(VERTEX v1, VERTEX v2, VERTEX v3, CHAR pixel_type, short col)
{
	int x1 = *v1.x;
	int y1 = *v1.y;

	int x2 = *v2.x;
	int y2 = *v2.y;

	int x3 = *v3.x;
	int y3 = *v3.y;

	auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
	auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) PlotPixel(glm::vec2(i, ny), pixel_type, col); };

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
	if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
	if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // swap values
		SWAP(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);    // Draw line from min to max points found on the y
									 // Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		SWAP(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3) return;
	}
	// Shade Characters is #x/- in order of luminescence
}

void Screen::DrawTriangleWireFrame(VERTEX v1, VERTEX v2, VERTEX v3, CHAR pixel_type, short col)
{
	// RENDERING LINES BETWEEN VERTICES
	DrawLine((int) *v1.x, (int) *v1.y, (int) *v2.x, (int) *v2.y, pixel_type, col);
	DrawLine((int) *v2.x, (int) *v2.y, (int) *v3.x, (int) *v3.y, pixel_type, col);
	DrawLine((int) *v3.x, (int) *v3.y, (int) *v1.x, (int) *v1.y, pixel_type, col);
}

VERTEX Screen::ViewPortTransform(VERTEX vertice)
{
	VERTEX newVert = vertice;
	glm::vec4 newPos = glm::vec4(((*vertice.x + 1.0f) / 2.0f) * SCR_WIDTH, ((*vertice.y + 1.0f) / 2.0f) * SCR_HEIGHT, *vertice.z, *vertice.w);
	newVert.SetXYZW(newPos);

	return newVert;
}

void Screen::StartFPSClock()
{
	startTimeFps = std::chrono::system_clock::now();
}

void Screen::EndFPSClock()
{
	endTimeFps = std::chrono::system_clock::now();
}