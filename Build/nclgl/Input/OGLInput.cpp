#include "OGLInput.h"

#include <nclgl\Keyboard.h>
#include <nclgl\Mouse.h>

#include <nclgl\WindowFactory.h>
#include <nclgl\OGLWindow.h>

OGLInput::OGLInput()
{
	window = static_cast<OGLWindow*>(WindowFactory::Instance()->GetWindow());
	keyboard = new Keyboard(window->GetHandle());
	mouse = new Mouse(window->GetHandle());
}


OGLInput::~OGLInput()
{
}
void OGLInput::ResetInput()
{

}

bool OGLInput::IsAction(InputToken action)
{
	switch (action)
	{
	case ESCAPE:
		return keyboard->KeyDown(KEYBOARD_X);
		break;
	case FORWARD:
		return keyboard->KeyDown(KEYBOARD_W);
		break;
	case BACKWARD:
		return keyboard->KeyDown(KEYBOARD_S);
		break;
	case LEFT:
		return keyboard->KeyDown(KEYBOARD_A);
		break;
	case RIGHT:
		return keyboard->KeyDown(KEYBOARD_D);
		break;
	case ACTION_A:
		return keyboard->KeyDown(KEYBOARD_SPACE);
		break;
	default:
		return false;
	}
}

void OGLInput::Update()
{
	keyboard->UpdateHolds();
	mouse->UpdateHolds();

	for (auto& event : window->keyboardEvents)
	{
		
		keyboard->Update(event);
	}

	for (auto& event : window->mouseEvents)
	{
		mouse->Update(event);
	}
}
