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

glm::vec3 Texture::GetPixelCol(glm::vec2 coord)
{
    // Use integer math for offset calculation and unsigned char for pixel data
    const size_t x = static_cast<size_t>(coord.x);
    const size_t y = static_cast<size_t>(coord.y);
    const size_t offset = m_BPP * (y * static_cast<size_t>(m_Width) + x);

    const unsigned char* buffer = reinterpret_cast<unsigned char*>(m_LocalBuffer);
    float r = buffer[offset];
    float g = buffer[offset + 1];
    float b = buffer[offset + 2];

    constexpr float inv255 = 1.0f / 255.0f;
    return glm::vec3(r * inv255, g * inv255, b * inv255);
}

glm::vec3 Texture::GetPixelCol(int x, int y)
{
    // Use integer math for offset calculation and unsigned char for pixel data
    const size_t offset = m_BPP * (static_cast<size_t>(y) * static_cast<size_t>(m_Width) + static_cast<size_t>(x));

    const unsigned char* buffer = reinterpret_cast<unsigned char*>(m_LocalBuffer);
    float r = buffer[offset];
    float g = buffer[offset + 1];
    float b = buffer[offset + 2];

    constexpr float inv255 = 1.0f / 255.0f;
    return glm::vec3(r * inv255, g * inv255, b * inv255);
}