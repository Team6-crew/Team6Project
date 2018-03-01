#include "WindowBase.h"



WindowBase::WindowBase(int sizeX, int sizeY)
{
	screenSize.x = (float)sizeX; 
	screenSize.y = (float)sizeY;
}


WindowBase::~WindowBase()
{
}

bool WindowBase::UpdateWindow()
{
	return true;
}

