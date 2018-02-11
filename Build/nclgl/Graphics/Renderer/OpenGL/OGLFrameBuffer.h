#pragma once

#include <nclgl\Graphics\FrameBufferBase.h>

#include <GL/glew.h>

#include <vector>

class TextureBase;

class OGLFrameBuffer : public FrameBufferBase
{
public:
	OGLFrameBuffer(TextureBase* colourTex, TextureBase* depthTex);
	OGLFrameBuffer(std::vector<TextureBase*> colourTex, TextureBase* depthTex);
	OGLFrameBuffer(TextureBase* depthTex);
	~OGLFrameBuffer();

	uint GetWidth() override;
	uint GetHeight() override;

	void Activate() override;
public:
	GLuint bufferID;

};

