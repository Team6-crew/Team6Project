#pragma once

#include "../../TextureBase.h"

#include <GL/glew.h>

class OGLTexture : public TextureBase
{
public:
	// Load a texture from a file
	OGLTexture(const std::string& filePath);
	// Create a texture of given width and height
	OGLTexture(Texture::Type type, int width, int height);
	~OGLTexture();

	void Bind(int textureUnit = 0) override;

	void SetTextureFiltering(bool nearest = false) override;
	void SetTextureWrapping(bool repeating = false) override;

	int TempGetID() override;
	GLuint GetID();
protected:
	bool LoadTexture(const std::string& filepath) override;

	GLuint textureID;
};