#include "FrameBufferFactory.h"

#include "OpenGL\OGLFrameBuffer.h"
#include "PS4\PS4FrameBuffer.h"

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
	#elif PSTATION4
		return new PS4FrameBuffer(colourTex, depthTex);
	#endif
}

FrameBufferBase* FrameBufferFactory::MakeFramebuffer(std::vector<TextureBase*> colourTex, TextureBase* depthTex)
{
	#ifdef WIN_OGL
		return new OGLFrameBuffer(colourTex, depthTex);
	#elif PSTATION4
	return new PS4FrameBuffer(colourTex, depthTex);
	#endif
}

FrameBufferBase* FrameBufferFactory::MakeFramebuffer(TextureBase* depthTex)
{
	#ifdef WIN_OGL
		return new OGLFrameBuffer(depthTex);
	#elif PSTATION4
	return new PS4FrameBuffer(depthTex);
	#endif
}