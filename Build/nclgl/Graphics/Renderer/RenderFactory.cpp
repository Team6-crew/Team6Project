#include "RenderFactory.h"

#include <nclgl\Window.h>

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
		//Initialise the Window
	if (!Window::Initialise(windowTitle, screenWidth, screenHeight, fullScreen))
	{
		Window::Destroy();
		assert(false);
	}
	else
	{
		return new OGLRenderer(Window::GetWindow());
	}
	#elif PSTATION4
	return new PS4Renderer();
	#endif

	// "Unrecognised Render Build"
	assert(false);
	return nullptr;
}