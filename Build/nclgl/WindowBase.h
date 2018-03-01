#pragma once

#include "Vector2.h"

class WindowBase
{
public:
	WindowBase(int x, int y);
	virtual ~WindowBase();

	nclgl::Maths::Vector2 GetScreenSize() { return screenSize; }
	virtual bool UpdateWindow();

protected:
	nclgl::Maths::Vector2 screenSize;
};

