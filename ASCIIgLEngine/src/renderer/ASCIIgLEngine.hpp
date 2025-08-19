#pragma once

#include "Vertex.hpp"
#include "VertexShader.hpp"

#include <vector>

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

#include <Windows.h>
#include <iostream>
#include <math.h>

class ASCIIgLEngine // this whole class is just something to hold all the functions I had no where to keep
{
private:

public:
	static glm::vec3 CalcNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, bool out); // calculates normals given 3 points
	static float CalcDist2D(glm::vec2 p1, glm::vec2 p2); // just calculates the distance between 2 2d points
	static glm::mat4 CalcModelMatrix(glm::vec3 position, glm::vec2 rotation, glm::vec3 size); // automatically calculate model matrix for rendering

	static bool pointLineCol2D(glm::vec2 p, glm::vec2 lineStart, glm::vec2 lineEnd); // gets if a point is colliding with a line
	static glm::vec3 lineCircleCol2D(glm::vec2 c, float r, glm::vec2 lineStart, glm::vec2 lineEnd); // gets if a line is colliding with a circle
	static bool pointCircle2D(glm::vec2 p, glm::vec2 c, float r); // gets if a point is colliding with a circle

	static glm::vec3 LineMeetsPlane(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 lineStart, glm::vec3 lineEnd); // gets where the line meets the plane
	static VERTEX HomogenousPlaneIntersect(VERTEX v1, VERTEX v2, int component, bool Near); // calculates where the line intersects the plane in clip space

	static void Clipping(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped, int component, bool Near); // this is a clipping algorithm that
	// divides polygons into new triangles based on if they are inside or outside the view
	static bool BackFaceCull(VERTEX v1, VERTEX v2, VERTEX v3, bool CCW); // returns if the triangles vertices are in the right winding order

	static void PerspectiveDivision(std::vector<VERTEX>& clipCoords, int i); // does perspective division on a list of vertices
	static void ClippingHelper(std::vector<VERTEX>& vertices, std::vector<VERTEX>& clipped); // helper function for clipping

	static float GrayScaleRGB(glm::vec3 rgb); // takes in a rgb value and spits out a grayscale float
	static CHAR_INFO GetColGlyph(float GreyScale); // gets the colour, and the character based on the greyscale
};

// just a bunch of enums for different colours, taken from a series on OneLoneCoder.com
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

// just a bunch of pixels types defined for easy use, pixel solid is percieved as brighter than pixel quarter
enum PIXEL_TYPE
{
	PIXEL_SOLID = 35,
	PIXEL_THREEQUARTERS = 120,
	PIXEL_HALF = 47,
	PIXEL_QUARTER = 45,
};