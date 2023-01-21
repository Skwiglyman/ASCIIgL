#pragma once

#include "stb_image/stb_image.h"

#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

class Texture
{
private:
	stbi_uc* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

public:
	std::string FilePath;
	std::string texType;

	Texture(const std::string& path, std::string type = "NULL");
	~Texture();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	glm::vec4 GetPixelCol(glm::vec2 xy);
	glm::vec4 GetPixelCol(int x, int y);
};