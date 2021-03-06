#pragma once

#include <nclgl\common.h> //uint
#include <string>

#include <ncltech\Memory Management\MemoryManager.h>

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

	//Temporarily need this until Framebuffers are integrated
	virtual int TempGetID() = 0;
protected:
	uint width;
	uint height;

	virtual void LoadTexture(const std::string& filepath) = 0;
	
	bool loadSuccess;
};

