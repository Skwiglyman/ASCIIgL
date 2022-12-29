#include "Screen.hpp"
#include "LineAlgo.hpp"
#include "ASCII_Math.hpp"


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


void Screen::DrawLine(int x1, int y1, int x2, int y2, short pixel_type, short col)
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


void Screen::DrawTriangleFill(std::vector<float> v1, std::vector<float> v2, std::vector<float> v3)
{
	std::vector<glm::vec2> triangle = FillTriangle(v1[0], v1[1], v2[0], v2[1], v3[0], v3[1]);

	for (size_t i = 0; i < triangle.size(); i++)
	{
		if (triangle[i].x >= 0 && triangle[i].x < SCR_WIDTH && triangle[i].y >= 0 && triangle[i].y < SCR_HEIGHT)
		{
			PlotPixel(triangle[i], v1[v1.size() - 2], v1[v1.size() - 1]);
		}
	}
	// Shade Characters is #x/- in order of luminescence
}

void Screen::DrawTriangleWireFrame(std::vector<float> v1, std::vector<float> v2, std::vector<float> v3)
{
	// RENDERING LINES BETWEEN VERTICES
	DrawLine(v1[0], v1[1], v2[0], v2[1], v1[v1.size() - 2], v1[v1.size() - 1]);
	DrawLine(v2[0], v2[1], v3[0], v3[1], v1[v1.size() - 2], v1[v1.size() - 1]);
	DrawLine(v3[0], v3[1], v1[0], v1[1], v1[v1.size() - 2], v1[v1.size() - 1]);
}


void Screen::DrawTrianglesWireFrame(VERTEX_SHADER VSHADER, std::vector<std::vector<float>> screenCoords)
{
	for (int i = 0; i < screenCoords.size(); i += 3)
	{
		DrawTriangleWireFrame(screenCoords[i], screenCoords[i + 1], screenCoords[i + 2]);
	}
}

void Screen::DrawTrianglesFill(VERTEX_SHADER VSHADER, std::vector<std::vector<float>> screenCoords)
{
	for (int i = 0; i < screenCoords.size(); i += 3)
	{
		DrawTriangleFill(screenCoords[i], screenCoords[i + 1], screenCoords[i + 2]);
	}
}

void Screen::nearClipping(std::vector<std::vector<float>>& vertices, std::vector<std::vector<float>>& clipped)
{
	for (int i = 0; i < vertices.size(); i += 3)
	{
		std::vector<int> inside;
		std::vector<int> outside;

		std::vector<float> temp[6];
		int newTri = 0;

		if (vertices[i][2] > 0) { inside.push_back(i); }
		else { outside.push_back(i); }

		if (vertices[i + 1][2] > 0) { inside.push_back(i + 1); }
		else { outside.push_back(i + 1); }

		if (vertices[i + 2][2] > 0) { inside.push_back(i + 2); }
		else { outside.push_back(i + 2); }

		if (inside.size() == 3)
		{
			clipped.push_back(vertices[inside[0]]);
			clipped.push_back(vertices[inside[1]]);
			clipped.push_back(vertices[inside[2]]);
			continue;
		}

		else if (inside.size() == 1)
		{
			std::vector<float> newPos1 = lineMeetsClippingBoundary(vertices[inside[0]], vertices[outside[0]]);
			std::vector<float> newPos2 = lineMeetsClippingBoundary(vertices[inside[0]], vertices[outside[1]]);

			temp[outside[0] - i] = newPos1;
			temp[outside[1] - i] = newPos2;
			temp[inside[0] - i] = vertices[inside[0]];
			newTri = 3;
		}

		else if (inside.size() == 2)
		{
			std::vector<float> newPos1 = lineMeetsClippingBoundary(vertices[inside[0]], vertices[outside[0]]);
			std::vector<float> newPos2 = lineMeetsClippingBoundary(vertices[inside[1]], vertices[outside[0]]);

			// triangle 1
			temp[inside[0] - i] = vertices[inside[0]];
			temp[inside[1] - i] = vertices[inside[1]];
			temp[outside[0] - i] = newPos1;

			// triangle 2
			temp[outside[0] - i + 3] = newPos2;
			temp[inside[0] - i + 3] = newPos1;
			temp[inside[1] - i + 3] = (vertices[inside[1]]);
			newTri = 6;
		}
		for (int k = 0; k < newTri; k++)
			clipped.push_back(temp[k]);
	}
}


void Screen::viewClipping(glm::vec3 planeP, glm::vec3 planeN, std::vector<std::vector<float>>& vertices, std::vector<std::vector<float>>& clipped, int i)
{
	std::vector<int> inside;
	std::vector<int> outside;

	std::vector<float> temp[6];
	int newTri = 0;

	auto dist = [&](glm::vec3 p)
	{
		return (planeN.x * p.x + planeN.y * p.y + planeN.z * p.z - glm::dot(planeN, planeP));
	};

	if (dist(getPos(vertices[i])) <= 0) { inside.push_back(i); }
	else { outside.push_back(i); }

	if (dist(glm::vec3(vertices[i + 1][0], vertices[i + 1][1], vertices[i + 1][2])) <= 0) { inside.push_back(i + 1); }
	else { outside.push_back(i + 1); }

	if (dist(glm::vec3(vertices[i + 2][0], vertices[i + 2][1], vertices[i + 2][2])) <= 0) { inside.push_back(i + 2); }
	else { outside.push_back(i + 2); }

	if (inside.size() == 3)
	{
		clipped.push_back(vertices[inside[0]]);
		clipped.push_back(vertices[inside[1]]);
		clipped.push_back(vertices[inside[2]]);
		return;
	}

	else if (inside.size() == 1)
	{
		std::vector<float> newPos1 = lineMeetsPlane(planeN, planeP, getPos(vertices[outside[0]]), getPos(vertices[inside[0]]));
		std::vector<float> newPos2 = lineMeetsPlane(planeN, planeP, getPos(vertices[outside[1]]), getPos(vertices[inside[0]]));

		temp[outside[0]-i] = newPos1;
		temp[outside[1]-i] = newPos2;
		temp[inside[0]-i] = vertices[inside[0]];
		newTri = 3;
	}

	else if (inside.size() == 2)
	{
		std::vector<float> newPos1 = lineMeetsPlane(planeN, planeP, getPos(vertices[outside[0]]), getPos(vertices[inside[0]]));
		std::vector<float> newPos2 = lineMeetsPlane(planeN, planeP, getPos(vertices[outside[0]]), getPos(vertices[inside[1]]));

		// triangle 1
		temp[inside[0]-i] = vertices[inside[0]];
		temp[inside[1]-i] = vertices[inside[1]];
		temp[outside[0]-i] = newPos1;

		// triangle 2
		temp[outside[0]-i + 3] = newPos2;
		temp[inside[0]-i + 3] = newPos1;
		temp[inside[1]-i + 3] = (vertices[inside[1]]);
		newTri = 6;
	}
	for (int k = 0; k < newTri; k++)
		clipped.push_back(temp[k]);
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

std::vector<float> Screen::clipToScreen(std::vector<float> p)
{
	std::vector<float> newVert;
	glm::vec4 newPos = glm::vec4(((p[0] + 1.0f) / 2.0f) * SCR_WIDTH, ((p[1] + 1.0f) / 2.0f) * SCR_HEIGHT, p[2], p[3]);
	vec4ToVert(newPos, &newVert);

	return newVert;
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

void Screen::viewClippingHelper(std::vector<std::vector<float>>& screenCoords, std::vector<std::vector<float>>& toDrawCoords)
{
	std::vector<std::vector<float>> c1, c2, c3, c4;

	for (int k = 0; k < screenCoords.size(); k += 3) 
	{ 
		viewClipping(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), screenCoords, c1, k); 
	}

	for (int k = 0; k < c1.size(); k += 3) 
	{ 
		viewClipping(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), c1, c2, k); 
	}

	for (int k = 0; k < c2.size(); k += 3) 
	{ 
		viewClipping(glm::vec3(0.0f, (float)SCR_HEIGHT - 1, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), c2, c3, k); 
	}
	
	for (int k = 0; k < c3.size(); k += 3) 
	{
		viewClipping(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), c3, c4, k); 
	}

	for (int k = 0; k < c4.size(); k += 3) 
	{ 
		viewClipping(glm::vec3((float)SCR_WIDTH - 1, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), c4, toDrawCoords, k); 
	}
}

bool Screen::BackFaceCull(std::vector<float> v1, std::vector<float> v2, std::vector<float> v3) // function that returns a negative if face is not culled
{
	glm::vec3 p1 = glm::vec3(v1[0], v1[1], v1[2]);
	glm::vec3 p2 = glm::vec3(v2[0], v2[1], v2[2]);
	glm::vec3 p3 = glm::vec3(v3[0], v3[1], v3[2]);

	glm::vec3 U = p2 - p1;
	glm::vec3 V = p3 - p1;

	float crossZ;

	if (CCW == true)
		crossZ = glm::cross(U, V).z;
	else
		crossZ = glm::cross(V, U).z;

	return crossZ > 0.0f;
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

