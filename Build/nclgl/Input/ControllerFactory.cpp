#include "ControllerFactory.h"

#include "PS4Input.h"
#include "OGLInput.h"

ControllerFactory::ControllerFactory()
{
}


ControllerFactory::~ControllerFactory()
{
}

InputBase* ControllerFactory::GetController()
{
#ifdef WIN_OGL
	static InputBase* controller = new  OGLInput();
	return controller;
#elif PSTATION4 
	static InputBase* controller = new  PS4Input();
	return controller;
#endif // WIN_OGL

}
