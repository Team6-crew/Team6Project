#pragma once

#include <nclgl/TSingleton.h>

#include <vector>

class FrameBufferBase;
class TextureBase;

class FrameBufferFactory : public TSingleton<FrameBufferFactory>
{
	friend class TSingleton<FrameBufferFactory>;
public:
	FrameBufferBase* MakeFramebuffer(TextureBase* depthTex, bool colour);
	FrameBufferBase* MakeFramebuffer(TextureBase* colourTex, TextureBase* depthTex);
	FrameBufferBase* MakeFramebuffer(std::vector<TextureBase*> colourTex, TextureBase* depthTex);

private:
	FrameBufferFactory();
	~FrameBufferFactory();
};

