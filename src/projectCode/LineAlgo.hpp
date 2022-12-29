#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

// This file is a header file that connects all of the line drawing functions, and filling algorithms to the main program

std::vector<glm::vec2> DrawLineBrensenham(int x1, int b, int x2, int y2);
std::vector<glm::vec2> FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
