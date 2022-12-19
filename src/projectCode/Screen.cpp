#include "Screen.hpp"
#include "LineAlgo.hpp"
#include "ASCII_Math.hpp"

#include <array>


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
	glm::vec3 newPos = glm::vec3(((p[0] + 1.0f) / 2.0f) * SCR_WIDTH, ((p[1] + 1.0f) / 2.0f) * SCR_HEIGHT, p[2]);
	vec3ToVert(newPos, &newVert);

	if (p.size() > 3)
	{
		for (size_t i = 3; i < p.size(); i++)
		{
			newVert.push_back(p[i]);
		}
	}
	

	return newVert;
}

void Screen::clipTriangleAgainstPlane(glm::vec3 planeP, glm::vec3 planeN, std::vector<std::vector<float>>& vertices, std::vector<std::vector<float>>& clipped, int i)
{
	int i1 = i;
	int i2 = i + 1;
	int i3 = i + 2;

	std::vector<int> inside;
	std::vector<int> outside;

	std::vector<float> temp[6];
	int newTri = 0;

	auto dist = [&](glm::vec3 p)
	{
		return (planeN.x * p.x + planeN.y * p.y + planeN.z * p.z - glm::dot(planeN, planeP));
	};

	auto index = [&](int i)
	{
		if (i == i1)
			return 0;
		else if (i == i2)
			return 1;
		else if (i == i3)
			return 2;
	};

	if (dist(glm::vec3(vertices[i1][0], vertices[i1][1], vertices[i1][2])) <= 0)
		inside.push_back(i1);
	else
		outside.push_back(i1);

	if (dist(glm::vec3(vertices[i2][0], vertices[i2][1], vertices[i2][2])) <= 0)
		inside.push_back(i2);
	else
		outside.push_back(i2);

	if (dist(glm::vec3(vertices[i3][0], vertices[i3][1], vertices[i3][2])) <= 0)
		inside.push_back(i3);
	else
		outside.push_back(i3);

	if (inside.size() == 3)
	{
		clipped.push_back(vertices[inside[0]]);
		clipped.push_back(vertices[inside[1]]);
		clipped.push_back(vertices[inside[2]]);
	}

	else if (inside.size() == 1)
	{
		glm::vec3 newPos1 = lineMeetsPlane(planeN, planeP, getPos(vertices[outside[0]]), getPos(vertices[inside[0]]));
		glm::vec3 newPos2 = lineMeetsPlane(planeN, planeP, getPos(vertices[outside[1]]), getPos(vertices[inside[0]]));

		temp[index(outside[0])] = {newPos1.x, newPos1.y, newPos1.z};
		temp[index(outside[1])] = { newPos2.x, newPos2.y, newPos2.z };
		temp[index(inside[0])] = vertices[inside[0]];
		newTri = 3;
	}

	else if (inside.size() == 2)
	{
		glm::vec3 newPos1 = lineMeetsPlane(planeN, planeP, getPos(vertices[outside[0]]), getPos(vertices[inside[0]]));
		glm::vec3 newPos2 = lineMeetsPlane(planeN, planeP, getPos(vertices[outside[0]]), getPos(vertices[inside[1]]));

		// triangle 1
		temp[index(inside[0])] = vertices[inside[0]];
		temp[index(inside[1])] = vertices[inside[1]];
		temp[index(outside[0])] = { newPos1.x, newPos1.y, newPos1.z };

		// triangle 2
		temp[index(outside[0]) + 3] = { newPos2.x, newPos2.y, newPos2.z };
		temp[index(inside[0]) + 3] = { newPos1.x, newPos1.y, newPos1.z };
		temp[index(inside[1]) + 3] = (vertices[inside[1]]);
		newTri = 6;
	}

	for (int i = 0; i < newTri; i++)
		clipped.push_back(temp[i]);
}

void Screen::localToWorld(VERTEX_SHADER VSHADER, std::vector<std::vector<float>>& localCoords, std::vector<std::vector<float>>& worldCoords)
{
	for (int i = 0; i < localCoords.size(); i++)
	{
		//transforming vertices using vertex shader
		std::vector<float> newVert = VSHADER.GLlocalToWorld(localCoords[i]);
		worldCoords.push_back(newVert);
	}
}

void Screen::worldToView(VERTEX_SHADER VSHADER, std::vector<std::vector<float>>& worldCoords, std::vector<std::vector<float>>& viewCoords)
{
	for (int i = 0; i < worldCoords.size(); i++)
	{
		//transforming vertices using vertex shader
		std::vector<float> newVert = VSHADER.GLworldToView(worldCoords[i]);
		viewCoords.push_back(newVert);
	}
}

void Screen::depthClipping(VERTEX_SHADER VSHADER, std::vector<std::vector<float>>& viewCoords, std::vector<std::vector<float>>& clippedViewCoords, float zNear, float zFar)
{
	// NEAR CLIPPING
	std::vector<std::vector<float>> newCoords1;
	for (int k = 0; k < viewCoords.size(); k += 3)
	{
		clipTriangleAgainstPlane(glm::vec3(0.0f, 0.0f, -zNear), glm::vec3(0.0f, 0.0f, 1.0f), viewCoords, newCoords1, k);
	}

	// FAR CLIPPING
	for (int k = 0; k < newCoords1.size(); k += 3)
	{
		clipTriangleAgainstPlane(glm::vec3(0.0f, 0.0f, -zFar), glm::vec3(0.0f, 0.0f, -1.0f), newCoords1, clippedViewCoords, k);
	}
}

void Screen::backFaceCulling(std::vector<std::vector<float>>& clippedViewCoords, std::vector<std::vector<float>>& backClippedCoords)
{
	for (int i = 0; i < clippedViewCoords.size(); i += 3)
	{
		if (BackFaceCull(clippedViewCoords[i], clippedViewCoords[i + 1], clippedViewCoords[i + 2]))
		{
			backClippedCoords.push_back(clippedViewCoords[i]);
			backClippedCoords.push_back(clippedViewCoords[i + 1]);
			backClippedCoords.push_back(clippedViewCoords[i + 2]);
		}
	}
}

void Screen::viewToClip(VERTEX_SHADER VSHADER, std::vector<std::vector<float>>& viewCoords, std::vector<std::vector<float>>& clipCoords)
{
	for (int i = 0; i < viewCoords.size(); i++)
	{
		//transforming vertices using vertex shader
		std::vector<float> newVert = VSHADER.GLviewToClip(viewCoords[i]);
		clipCoords.push_back(newVert);
	}
}

void Screen::perspectiveDivision(std::vector<std::vector<float>>& clipCoords)
{
	for (int i = 0; i < clipCoords.size(); i++)
	{
		clipCoords[i][0] /= clipCoords[i][3];
		clipCoords[i][1] /= clipCoords[i][3];
		clipCoords[i][2] /= clipCoords[i][3];
		clipCoords[i][3] = 1.0f;
	}
}

void Screen::NDCToScreen(std::vector<std::vector<float>>& ndcCoords, std::vector<std::vector<float>>& screenCoords)
{
	for (int i = 0; i < ndcCoords.size(); i++)
	{
		// CHANGING FROM NDC SPCE TO SCREEN SPACE
		std::vector<float> screenCoord = clipToScreen(ndcCoords[i]);
		screenCoords.push_back(screenCoord);
	}
}

void Screen::DrawTriangles(VERTEX_SHADER VSHADER, std::vector<std::vector<float>> screenCoords)
{
	/*clipTriangleAgainstPlane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), viewCoords, clippedViewCoords, k);
	clipTriangleAgainstPlane(glm::vec3(0.0f, (float)SCR_HEIGHT - 1, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), viewCoords, clippedViewCoords, k);
	clipTriangleAgainstPlane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), viewCoords, clippedViewCoords, k);
	clipTriangleAgainstPlane(glm::vec3((float)SCR_WIDTH - 1, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), viewCoords, clippedViewCoords, k);*/

	for (int i = 0; i < screenCoords.size(); i += 3)
	{
		// RENDERING LINES BETWEEN VERTICES
		DrawLine(screenCoords[i][0], screenCoords[i][1], screenCoords[i + 1][0], screenCoords[i + 1][1]);
		DrawLine(screenCoords[i + 1][0], screenCoords[i + 1][1], screenCoords[i + 2][0], screenCoords[i + 2][1]);
		DrawLine(screenCoords[i + 2][0], screenCoords[i + 2][1], screenCoords[i][0], screenCoords[i][1]);
	}
}

float Screen::BackFaceCull(std::vector<float> v1, std::vector<float> v2, std::vector<float> v3) // function that returns a negative if face is not culled
{
	// BACKFACE CULLING (counter clockwise)
	// calculate normals using vertices in VIEW SPACE

	glm::vec3 p1 = glm::vec3(glm::vec4(v1[0], v1[1], v1[2], 1.0f));
	glm::vec3 p2 = glm::vec3(glm::vec4(v2[0], v2[1], v2[2], 1.0f));
	glm::vec3 p3 = glm::vec3(glm::vec4(v3[0], v3[1], v3[2], 1.0f));

	glm::vec3 normal = calcNormal(p1, p2, p3, true); // Getting normal of surface triangle

	return glm::dot(normal, p1) < 0.0f;
}
