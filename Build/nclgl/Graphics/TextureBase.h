#pragma once

#include <nclgl\common.h> //uint

class TextureBase
{
public:
	TextureBase();
	~TextureBase();

	uint GetWidth() const;
	uint GetHeight() const;

protected:
	uint width;
	uint height;
};

