#ifdef WIN_OGL
#pragma once

#include <nclgl\Graphics\TextureBase.h>
#include <GL/glew.h>

// TODO: This could probably extend or derive from OGLTexture
// but it was making a mess so seperating for now

class OGLTextureArray : public TextureBase
{
public:
	OGLTextureArray(Texture::Type type, int width, int height);
	~OGLTextureArray();


	void Bind(int textureUnit = 0) override;

	void SetTextureFiltering(bool nearest = false) override;
	void SetTextureWrapping(bool repeating = false) override;


	int GetID() { return textureID; }
protected:
	bool LoadTexture(const std::string& filepath) override;
	GLuint textureID;
};

#endif