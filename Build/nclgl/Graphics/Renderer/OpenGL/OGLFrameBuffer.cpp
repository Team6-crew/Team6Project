#include "OGLFrameBuffer.h"

#include "OGLTexture.h"
#include "OGLTextureArray.h"
#include <nclgl\NCLDebug.h>

DEFINE_HEAP(OGLFrameBuffer, "Graphics");

OGLFrameBuffer::OGLFrameBuffer(TextureBase* colourTex, TextureBase* depthTex)
{
	glGenFramebuffers(1, &bufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, static_cast<OGLTexture*>(colourTex)->GetID(), 0);
	if (depthTex) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, static_cast<OGLTexture*>(depthTex)->GetID(), 0);
	}

	//Validate our framebuffer
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		NCLERROR("Unable to create Screen Framebuffer!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OGLFrameBuffer::OGLFrameBuffer(std::vector<TextureBase*> colourTex, TextureBase* depthTex)
{
	glGenFramebuffers(1, &bufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
	for (int i = 0; i < colourTex.size(); ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, static_cast<OGLTexture*>(colourTex[i])->GetID(), 0);
	}
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, static_cast<OGLTexture*>(depthTex)->GetID(), 0);

	//Validate our framebuffer
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		NCLERROR("Unable to create Screen Framebuffer!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OGLFrameBuffer::OGLFrameBuffer(TextureBase* depthTex, bool colour)
{
	glGenFramebuffers(1, &bufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
	if (colour) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, static_cast<OGLTextureArray*>(depthTex)->GetID(), 0);
	}
	else {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, static_cast<OGLTextureArray*>(depthTex)->GetID(), 0);
	}

	glDrawBuffers(0, GL_NONE);

	//Validate our framebuffer
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		NCLERROR("Unable to create Screen Framebuffer!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void OGLFrameBuffer::ChangeColourAttachment(TextureBase* attachment)
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, static_cast<OGLTexture*>(attachment)->GetID(), 0);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OGLFrameBuffer::~OGLFrameBuffer()
{
	glDeleteFramebuffers(1, &bufferID);
}



uint OGLFrameBuffer::GetWidth()
{
	return 1;
}

uint OGLFrameBuffer::GetHeight()
{
	return 1;
}
	
void OGLFrameBuffer::Activate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
}