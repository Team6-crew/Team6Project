#include "FrameBufferFactory.h"

#include "OpenGL\OGLFrameBuffer.h"

FrameBufferFactory::FrameBufferFactory()
{
}


FrameBufferFactory::~FrameBufferFactory()
{
}

FrameBufferBase* FrameBufferFactory::MakeFramebuffer(TextureBase* colourTex, TextureBase* depthTex)
{
	#ifdef WIN_OGL
		return new OGLFrameBuffer(colourTex, depthTex);
	#endif
}

FrameBufferBase* FrameBufferFactory::MakeFramebuffer(std::vector<TextureBase*> colourTex, TextureBase* depthTex)
{
	#ifdef WIN_OGL
		return new OGLFrameBuffer(colourTex, depthTex);
	#endif
}

FrameBufferBase* FrameBufferFactory::MakeFramebuffer(TextureBase* depthTex, bool colour)
{
	#ifdef WIN_OGL
		return new OGLFrameBuffer(depthTex, colour);
	#endif
}

