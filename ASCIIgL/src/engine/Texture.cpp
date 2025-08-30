#include <ASCIIgL/engine/Texture.hpp>

#include <ASCIIgL/engine/Logger.hpp>
#include <stb_image/stb_image.h>
#include <algorithm>

// PIMPL Implementation class that contains all stb_image-related code
class Texture::Impl
{
public:
    stbi_uc* m_LocalBuffer; // buffer that holds all of the colour data
    int m_Width, m_Height, m_BPP;
    std::string FilePath;

    Impl(const std::string& path);
    ~Impl();
    float GetPixelCol(int x, int y) const;
    float GetPixelColFiltered(float u, float v) const;
};

Texture::Impl::Impl(const std::string& path)
    : FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
    stbi_set_flip_vertically_on_load(0);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 1); // m_BPP being set to 4 even for grayscale for some reason
}

Texture::Impl::~Impl()
{
    // deletes the buffer
    if (m_LocalBuffer) {
        stbi_image_free(m_LocalBuffer);
    }
}

float Texture::Impl::GetPixelCol(int x, int y) const
{
    // Use integer math for offset calculation and unsigned char for pixel data
    const size_t offset = static_cast<size_t>(y) * static_cast<size_t>(m_Width) + static_cast<size_t>(x);
    const unsigned char* buffer = reinterpret_cast<unsigned char*>(m_LocalBuffer);
    unsigned int shade = buffer[offset];

    constexpr float inv255 = 1.0f / 255.0f;
    return shade * inv255;
}

// Add bilinear filtered texture sampling method
float Texture::Impl::GetPixelColFiltered(float u, float v) const
{
    // Convert UV coordinates to pixel coordinates
    float x = u * (m_Width - 1);
    float y = v * (m_Height - 1);
    
    // Get integer and fractional parts
    int x0 = static_cast<int>(x);
    int y0 = static_cast<int>(y);
    int x1 = std::min(x0 + 1, m_Width - 1);
    int y1 = std::min(y0 + 1, m_Height - 1);
    
    float fx = x - x0;
    float fy = y - y0;
    
    // Clamp coordinates to texture bounds
    x0 = std::max(0, std::min(x0, m_Width - 1));
    y0 = std::max(0, std::min(y0, m_Height - 1));
    
    // Sample the four surrounding pixels
    const unsigned char* buffer = reinterpret_cast<unsigned char*>(m_LocalBuffer);
    constexpr float inv255 = 1.0f / 255.0f;
    
    float c00 = buffer[y0 * m_Width + x0] * inv255;
    float c10 = buffer[y0 * m_Width + x1] * inv255;
    float c01 = buffer[y1 * m_Width + x0] * inv255;
    float c11 = buffer[y1 * m_Width + x1] * inv255;
    
    // Bilinear interpolation
    float c0 = c00 * (1.0f - fx) + c10 * fx;
    float c1 = c01 * (1.0f - fx) + c11 * fx;
    
    return c0 * (1.0f - fy) + c1 * fy;
}

// Texture public interface implementation
Texture::Texture(const std::string& path, const std::string type)
    : texType(type), pImpl(std::make_unique<Impl>(path))
{
}

Texture::~Texture() = default;

Texture::Texture(Texture&& other) noexcept 
    : texType(std::move(other.texType)), pImpl(std::move(other.pImpl))
{
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        texType = std::move(other.texType);
        pImpl = std::move(other.pImpl);
    }
    return *this;
}

int Texture::GetWidth() const
{
    return pImpl->m_Width;
}

int Texture::GetHeight() const
{
    return pImpl->m_Height;
}

std::string Texture::GetFilePath() const
{
    return pImpl->FilePath;
}

float Texture::GetPixelCol(int x, int y) const
{
    return pImpl->GetPixelCol(x, y);
}

float Texture::GetPixelColFiltered(float u, float v) const
{
    return pImpl->GetPixelColFiltered(u, v);
}