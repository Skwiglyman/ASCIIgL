#pragma once

#include "Vertex.hpp"
#include "Texture.hpp"

#include <vector>

class Mesh
{
private:

public:
	std::vector<VERTEX> vertices;
	Texture texture;

	Mesh(std::vector<Vertex> inVerts, Texture inTex);
	~Mesh();

};