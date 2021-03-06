#pragma once

#include <nclgl\Graphics\TextureBase.h>
#include <GL/glew.h>

// TODO: This could probably extend or derive from OGLTexture
// but it was making a mess so seperating for now

class OGLTextureArray : public TextureBase
{
	DECLARE_HEAP;
public:
	OGLTextureArray(Texture::Type type, int width, int height);
	~OGLTextureArray();


	void Bind(int textureUnit = 0) override;

	void SetTextureFiltering(bool nearest = false) override;
	void SetTextureWrapping(bool repeating = false) override;


	int TempGetID() override;
	int GetID() { return textureID; }
protected:
	void LoadTexture(const std::string& filepath) override;
	GLuint textureID;
};

