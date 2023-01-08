#pragma once

#include "stb_image/stb_image.h"

#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Texture
{
private:
	stbi_uc* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

public:
	std::string FilePath;

	Texture(const std::string& path);
	~Texture();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	glm::vec4 GetPixel(glm::vec2 xy);
};