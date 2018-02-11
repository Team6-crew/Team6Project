#pragma once

#include <nclgl\common.h> // uint

class TextureBase;

class FrameBufferBase
{
public:
	FrameBufferBase();
	~FrameBufferBase();

	virtual uint GetWidth() = 0;
	virtual uint GetHeight() = 0;

	virtual void Activate() = 0;
};

