#include "RenderFactory.h"

#include <nclgl\WindowFactory.h>
#include <nclgl\OGLWindow.h>
#include "OpenGL\OGLRenderer.h"
#include "PS4\PS4Renderer.h"
#include "../RenderConstants.h"

#include <cassert>

using namespace RenderConstants;

RenderFactory::RenderFactory()
{
}


RenderFactory::~RenderFactory()
{
}

RenderBase* RenderFactory::MakeRenderer()
{
	#ifdef WIN_OGL
		//Initialise the OGLWindow
	WindowFactory::Instance()->MakeWindow(windowTitle, screenWidth, screenHeight, fullScreen);
	return new OGLRenderer(static_cast<OGLWindow*>(WindowFactory::Instance()->GetWindow()));
	#elif PSTATION4
	return new PS4Renderer();
	#endif

	// "Unrecognised Render Build"
	assert(false);
	return nullptr;
}