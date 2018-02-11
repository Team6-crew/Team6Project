#pragma once

#include <nclgl\common.h> // uint

class FrameBufferBase
{
public:
	FrameBufferBase();
	~FrameBufferBase();

	virtual uint GetWidth() = 0;
	virtual uint GetHeight() = 0;


};

