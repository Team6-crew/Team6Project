#pragma once

#include <nclgl\Graphics\FrameBufferBase.h>

#include <GL/glew.h>

#include <vector>

class TextureBase;

#include <ncltech\Memory Management\MemoryManager.h>

class OGLFrameBuffer : public FrameBufferBase
{
	DECLARE_HEAP;
public:
	OGLFrameBuffer(TextureBase* colourTex, TextureBase* depthTex);
	OGLFrameBuffer(std::vector<TextureBase*> colourTex, TextureBase* depthTex);
	OGLFrameBuffer(TextureBase* depthTex, bool colour);
	void ChangeColourAttachment(TextureBase* attachment) override;
	~OGLFrameBuffer();

	uint GetWidth() override;
	uint GetHeight() override;

	void Activate() override;
public:
	GLuint bufferID;

};

