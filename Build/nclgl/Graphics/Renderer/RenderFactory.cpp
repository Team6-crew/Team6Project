#include "RenderFactory.h"

#include "OGLRenderer.h"
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
	#endif

	// "Unrecognised Render Build"
	assert(false);
	return nullptr;
}