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


void Screen::DrawTrianglesWireFrame(std::vector<VERTEX> screenCoords)
{
	for (int i = 0; i < screenCoords.size(); i += 3)
	{
		DrawTriangleWireFrame(screenCoords[i], screenCoords[i + 1], screenCoords[i + 2]);
	}
}

void Screen::DrawTrianglesFill(std::vector<VERTEX> screenCoords)
{
	for (int i = 0; i < screenCoords.size(); i += 3)
	{
		DrawTriangleFill(screenCoords[i], screenCoords[i + 1], screenCoords[i + 2]);
	}
}

void Screen::Clipping(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped, int component, bool Near)
{
	for (int i = 0; i < vertices.size(); i += 3)
	{
		std::vector<int> inside;
		std::vector<int> outside;

		VERTEX temp[6];
		int newTri = 0;

		if (Near == true)
		{
			if (vertices[i].data.at(component) > -*vertices[i].w) { inside.push_back(i); }
			else { outside.push_back(i); }

			if (vertices[i + 1].data.at(component) > -*vertices[i + 1].w) { inside.push_back(i + 1); }
			else { outside.push_back(i + 1); }

			if (vertices[i + 2].data.at(component) > -*vertices[i + 2].w) { inside.push_back(i + 2); }
			else { outside.push_back(i + 2); }
		}
		
		else
		{
			if (vertices[i].data.at(component) < *vertices[i].w) { inside.push_back(i); }
			else { outside.push_back(i); }

			if (vertices[i + 1].data.at(component) < *vertices[i + 1].w) { inside.push_back(i + 1); }
			else { outside.push_back(i + 1); }

			if (vertices[i + 2].data.at(component) < *vertices[i + 2].w) { inside.push_back(i + 2); }
			else { outside.push_back(i + 2); }
		}

		if (inside.size() == 3)
		{
			clipped.push_back(vertices[inside[0]]);
			clipped.push_back(vertices[inside[1]]);
			clipped.push_back(vertices[inside[2]]);
			continue;
		}

		else if (inside.size() == 1)
		{
			VERTEX newPos1 = homogenousPlaneIntersect(vertices[inside[0]], vertices[outside[0]], component, Near);
			VERTEX newPos2 = homogenousPlaneIntersect(vertices[inside[0]], vertices[outside[1]], component, Near);

			temp[outside[0] - i] = newPos1;
			temp[outside[1] - i] = newPos2;
			temp[inside[0] - i] = vertices[inside[0]];
			newTri = 3;
		}

		else if (inside.size() == 2)
		{
			VERTEX newPos1 = homogenousPlaneIntersect(vertices[inside[0]], vertices[outside[0]], component, Near);
			VERTEX newPos2 = homogenousPlaneIntersect(vertices[inside[1]], vertices[outside[0]], component, Near);

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


//void Screen::ViewClipping(glm::vec3 planeP, glm::vec3 planeN, std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped)
//{
//	for (int i = 0; i < vertices.size(); i += 3)
//	{
//		std::vector<int> inside;
//		std::vector<int> outside;
//
//		VERTEX temp[6];
//		int newTri = 0;
//
//		auto dist = [&](glm::vec3 p)
//		{
//			return (planeN.x * p.x + planeN.y * p.y + planeN.z * p.z - glm::dot(planeN, planeP));
//		};
//
//		if (dist(vertices[i].GetXYZ()) <= 0) { inside.push_back(i); }
//		else { outside.push_back(i); }
//
//		if (dist(vertices[i + 1].GetXYZ()) <= 0) { inside.push_back(i + 1); }
//		else { outside.push_back(i + 1); }
//
//		if (dist(vertices[i + 2].GetXYZ()) <= 0) { inside.push_back(i + 2); }
//		else { outside.push_back(i + 2); }
//
//		if (inside.size() == 3)
//		{
//			clipped.push_back(vertices[inside[0]]);
//			clipped.push_back(vertices[inside[1]]);
//			clipped.push_back(vertices[inside[2]]);
//			continue;
//		}
//
//		else if (inside.size() == 1)
//		{
//			VERTEX newPos1 = lineMeetsPlane(planeN, planeP, vertices[outside[0]].GetXYZ(), vertices[inside[0]].GetXYZ());
//			VERTEX newPos2 = lineMeetsPlane(planeN, planeP, vertices[outside[1]].GetXYZ(), vertices[inside[0]].GetXYZ());
//
//			temp[outside[0] - i] = newPos1;
//			temp[outside[1] - i] = newPos2;
//			temp[inside[0] - i] = vertices[inside[0]];
//			newTri = 3;
//		}
//
//		else if (inside.size() == 2)
//		{
//			VERTEX newPos1 = lineMeetsPlane(planeN, planeP, vertices[outside[0]].GetXYZ(), vertices[inside[0]].GetXYZ());
//			VERTEX newPos2 = lineMeetsPlane(planeN, planeP, vertices[outside[0]].GetXYZ(), vertices[inside[1]].GetXYZ());
//
//			// triangle 1
//			temp[inside[0] - i] = vertices[inside[0]];
//			temp[inside[1] - i] = vertices[inside[1]];
//			temp[outside[0] - i] = newPos1;
//
//			// triangle 2
//			temp[outside[0] - i + 3] = newPos2;
//			temp[inside[0] - i + 3] = newPos1;
//			temp[inside[1] - i + 3] = (vertices[inside[1]]);
//			newTri = 6;
//		}
//		for (int k = 0; k < newTri; k++)
//			clipped.push_back(temp[k]);
//	}
//}

void Screen::VertexTransform(VERTEX_SHADER VSHADER, std::vector<VERTEX>& localCoords, std::vector<VERTEX>& clipCoords)
{
	for (int i = 0; i < localCoords.size(); i++)
	{
		//transforming vertices using vertex shader
		VERTEX newVert = VSHADER.GLUse(localCoords[i]);
		clipCoords.push_back(newVert);
	}
}

void Screen::BackFaceCulling(std::vector<VERTEX>& clippedViewCoords, std::vector<VERTEX>& backClippedCoords)
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

void Screen::ClippingHelper(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped)
{
	std::vector<VERTEX> c1, c2, c3, c4, c5;

	Clipping(vertices, c1, 2, true);
	Clipping(c1, c2, 2, false);

	Clipping(c2, c3, 1, true);
	Clipping(c3, c4, 1, false);

	Clipping(c4, c5, 0, true);
	Clipping(c5, clipped, 0, false);
}

VERTEX Screen::ClipToScreen(VERTEX vertice)
{
	VERTEX newVert = vertice;
	glm::vec4 newPos = glm::vec4(((*vertice.x + 1.0f) / 2.0f) * SCR_WIDTH, ((*vertice.y + 1.0f) / 2.0f) * SCR_HEIGHT, *vertice.z, *vertice.w);
	newVert.SetXYZW(newPos);

	return newVert;
}

void Screen::NDCToScreen(std::vector<VERTEX>& ndcCoords, std::vector<VERTEX>& screenCoords)
{
	for (int i = 0; i < ndcCoords.size(); i++)
	{
		// CHANGING FROM NDC SPCE TO SCREEN SPACE
		VERTEX newVert = ClipToScreen(ndcCoords[i]);
		screenCoords.push_back(newVert);
	}
}

//void Screen::ViewClippingHelper(std::vector<VERTEX>& screenCoords, std::vector<VERTEX>& toDrawCoords)
//{
//	std::vector<VERTEX> c1, c2, c3, c4;
//
//	ViewClipping(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), screenCoords, c1); 
//	ViewClipping(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), c1, c2); 
//	ViewClipping(glm::vec3(0.0f, (float)SCR_HEIGHT - 1, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), c2, c3); 
//	ViewClipping(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), c3, c4); 
//	ViewClipping(glm::vec3((float)SCR_WIDTH - 1, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), c4, toDrawCoords); 
//}

bool Screen::BackFaceCull(VERTEX v1, VERTEX v2, VERTEX v3) // function that returns a negative if face is not culled
{
	glm::vec3 p1 = glm::vec3(*v1.x, *v1.y, *v1.z);
	glm::vec3 p2 = glm::vec3(*v2.x, *v2.y, *v2.z);
	glm::vec3 p3 = glm::vec3(*v3.x, *v3.y, *v3.z);

	glm::vec3 U = p2 - p1;
	glm::vec3 V = p3 - p1;

	float crossZ;

	if (CCW == true)
		crossZ = glm::cross(U, V).z;
	else
		crossZ = glm::cross(V, U).z;

	return crossZ > 0.0f;
}

void Screen::PerspectiveDivision(std::vector<VERTEX>& clipCoords)
{
	for (int i = 0; i < clipCoords.size(); i++)
	{
		*clipCoords[i].x /= *clipCoords[i].w;
		*clipCoords[i].y /= *clipCoords[i].w;
		*clipCoords[i].z /= *clipCoords[i].w;
		*clipCoords[i].w = 1.0f;
	}
}