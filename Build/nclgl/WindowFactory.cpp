#include "WindowFactory.h"



class WindowBase;


#include <nclgl\Graphics\Renderer\OpenGL\OGLWindow.h>
#include "WindowBase.h"

WindowFactory::WindowFactory()
{
	window = nullptr;
}


WindowFactory::~WindowFactory()
{
	delete window;
}


WindowBase * WindowFactory::MakeWindow(std::string title, int sizeX, int sizeY, bool fullScreen)
{
#ifdef WIN_OGL
	if (!window)
	{
		window = new OGLWindow(title, sizeX, sizeY, fullScreen);
		return window;
	}
	
#elif PSTATION4
	window = new WindowBase(sizeX, sizeY);
	return window;
#endif // WIN_OGL

}

WindowBase * WindowFactory::GetWindow()
{
	return window;
}
