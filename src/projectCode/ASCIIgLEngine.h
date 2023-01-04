#pragma once

#include "Vertex.hpp"
#include "VertexShader.hpp"

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void Clipping(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped, int component, bool Near, int i);
bool BackFaceCull(VERTEX v1, VERTEX v2, VERTEX v3, bool CCW);

void PerspectiveDivision(std::vector<VERTEX>& clipCoords, int i);
void ClippingHelper(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped, int i);

glm::vec3 CalcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, bool out); // calculates normals given 3 points

VERTEX LineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd);
VERTEX HomogenousPlaneIntersect(VERTEX v1, VERTEX v2, int component, bool Near);

std::vector<glm::vec2> DrawLineBrensenham(int x1, int b, int x2, int y2);
std::vector<glm::vec2> FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

enum COLOUR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007,
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 35,
	PIXEL_THREEQUARTERS = 120,
	PIXEL_HALF = 47,
	PIXEL_QUARTER = 45,
};