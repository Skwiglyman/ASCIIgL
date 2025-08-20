#pragma once

// includes from downloaded libraries
#include "../vendor/stb_image/stb_image.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

// includes from default C++ libraries
#include <iostream>
#include <string>

class Texture // this class loads a texture from a path and holds its data in a 1d buffer
{
private:
	stbi_uc* m_LocalBuffer; // buffer that holds all of the colour data
	int m_Width, m_Height, m_BPP;

public:
	std::string FilePath;
	std::string texType;

	// type is for model drawing, as they have texture types
	Texture(const std::string& path, std::string type = "NULL");
	~Texture();

	// getters for width and height
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	// returns an rgba between 0 and 1
	glm::vec3 GetPixelCol(glm::vec2 xy);
	glm::vec3 GetPixelCol(int x, int y);
};