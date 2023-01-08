#include "Texture.hpp"

Texture::Texture(const std::string& path)
	: FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
}

Texture::~Texture()
{
	stbi_image_free(m_LocalBuffer);
}

glm::vec4 Texture::GetPixel(glm::vec2 coord)
{
	const stbi_uc* pixelOffset = m_LocalBuffer + (4 * (size_t(coord.y) * size_t(m_Width) + size_t(coord.x)));
	stbi_uc  r = pixelOffset[0];
	stbi_uc  g = pixelOffset[1];
	stbi_uc  b = pixelOffset[2];
	stbi_uc  a = m_BPP >= 4 ? pixelOffset[3] : 0xff;

	return glm::vec4(r, g, b, a);
}
