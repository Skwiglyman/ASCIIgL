#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <memory>

class Texture // this class loads a texture from a path and holds its data in a 1d buffer
{
public:
	std::string texType;

	// type is for model drawing, as they have texture types
	Texture(const std::string& path, std::string type = "NULL");
	~Texture();

	// Move constructor and assignment for PIMPL
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	// Delete copy constructor and assignment (or implement if needed)
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// getters for width and height
	int GetWidth() const;
	int GetHeight() const;
	std::string GetFilePath() const;

	// returns an rgba between 0 and 1
	float GetPixelCol(int x, int y) const;
	
	// returns bilinear filtered pixel value using UV coordinates (0-1 range)
	float GetPixelColFiltered(float u, float v) const;

private:
	// Forward declaration for PIMPL pattern
	class Impl;
	std::unique_ptr<Impl> pImpl;
};