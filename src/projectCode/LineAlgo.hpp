#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

// This file is a header file that connects all of the line drawing functions, and filling algorithms to the main program

std::vector<glm::vec2> DrawLine(int x1, int y1, int x2, int y2);