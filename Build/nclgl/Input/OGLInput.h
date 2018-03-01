#ifdef WIN_OGL



#pragma once

#include "InputBase.h"

class Keyboard;
class Mouse;
class OGLWindow;

class OGLInput : public InputBase
{
public:
	OGLInput();
	~OGLInput();


	void ResetInput() override;

	bool IsAction(InputToken action) override;
	void Update() override;
protected:
	Keyboard*	keyboard;
	Mouse*		mouse;
	OGLWindow*  window;
};

#endif // WIN_OGL