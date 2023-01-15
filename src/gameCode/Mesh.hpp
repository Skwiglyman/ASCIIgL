#pragma once

#include "..\renderingEngineCode\Vertex.hpp"
#include "..\renderingEngineCode\Texture.hpp"

#include <vector>

class Mesh
{
private:

public:
	std::vector<VERTEX> vertices;
	std::vector<Texture*> textures;

	Mesh(std::vector<VERTEX> inVerts, std::vector<Texture*> inTex);
	~Mesh();

};