#include "Texture.hpp"

Texture::Texture(const std::string& path, const std::string type)
	: FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), texType(type)
{
	// does not flip uvs on the image
	stbi_set_flip_vertically_on_load(0);
	// loading the image onto the buffer
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
}

Texture::~Texture()
{
	// deletes the buffer
	stbi_image_free(m_LocalBuffer);
}

glm::vec4 Texture::GetPixelCol(glm::vec2 coord)
{
	// gets the rgba value from the buffer, and returns it in a 0-1 format instead of 0-255
	const int offset = m_BPP * (size_t(coord.y) * size_t(m_Width) + size_t(coord.x));
	float  r = m_LocalBuffer[offset];
	float  g = m_LocalBuffer[offset + 1];
	float  b = m_LocalBuffer[offset + 2];
	float  a = m_BPP >= 4 ? m_LocalBuffer[offset + 3] : 0xFF;

	return glm::vec4(r / 255, g / 255, b / 255, a / 255);
}

glm::vec4 Texture::GetPixelCol(int x, int y)
{
	// gets the rgba value from the buffer, and returns it in a 0-1 format instead of 0-255
	const int offset = m_BPP * (y * m_Width + x);
	float  r = m_LocalBuffer[offset];
	float  g = m_LocalBuffer[offset + 1];
	float  b = m_LocalBuffer[offset + 2];
	float  a = m_BPP >= 4 ? m_LocalBuffer[offset + 3] : 0xFF;

	return glm::vec4(r / 255, g / 255, b / 255, a / 255);
}