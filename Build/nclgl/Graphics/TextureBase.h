#pragma once

#include <nclgl\common.h> //uint
#include <string>

namespace Texture
{
	enum Type { COLOUR, DEPTH, DEPTH_ARRAY };
}


class TextureBase
{
public:
	TextureBase();
	~TextureBase();

	uint GetWidth() const;
	uint GetHeight() const;

	virtual void Bind(int textureUnit = 0) = 0;

	virtual void SetTextureFiltering(bool nearest = false) = 0;
	virtual void SetTextureWrapping(bool repeating = false)  = 0;
protected:
	uint width;
	uint height;

	virtual bool LoadTexture(const std::string& filepath) = 0;
	
	bool loadSuccess;
};

