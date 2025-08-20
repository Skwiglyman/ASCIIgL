#include "Texture.hpp"

#include <engine/Logger.hpp>

Texture::Texture(const std::string& path, const std::string type)
	: FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), texType(type)
{
	stbi_set_flip_vertically_on_load(0);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 1); // m_BPP being set to 4 even for grayscale for some reason
}

Texture::~Texture()
{
	// deletes the buffer
	stbi_image_free(m_LocalBuffer);
}

float Texture::GetPixelCol(int x, int y)
{
    // Use integer math for offset calculation and unsigned char for pixel data
    const size_t offset = static_cast<size_t>(y) * static_cast<size_t>(m_Width) + static_cast<size_t>(x);
    const unsigned char* buffer = reinterpret_cast<unsigned char*>(m_LocalBuffer);
    unsigned int shade = buffer[offset];

    constexpr float inv255 = 1.0f / 255.0f;
    return shade * inv255;
}