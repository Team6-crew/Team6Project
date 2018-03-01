#include "PS4Window.h"

#include <nclgl\Input\ControllerFactory.h>
#include <nclgl\Input\InputBase.h>

PS4Window::PS4Window() :
	WindowBase(0, 0)
{
}


PS4Window::~PS4Window()
{
}

bool PS4Window::UpdateWindow()
{
	ControllerFactory::Instance()->GetController()->Update();
	return true;
}