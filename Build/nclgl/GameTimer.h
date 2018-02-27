/******************************************************************************
Class:GameTimer
Author:Rich Davison
Description:Wraps Windows PerformanceCounter. GameTimers keep track of how much
time has passed since they were last polled - so you could use multiple
GameTimers to trigger events at different time periods. 

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#ifdef WIN_OGL

#include "Windows.h"
class GameTimer	{
public:
	GameTimer(void);
	~GameTimer(void) {}

	//How many milliseconds have passed since the GameTimer was created
	float	GetMS();

	//How many milliseconds have passed since GetTimedMS was last called
	float	GetTimedMS();

protected:
	LARGE_INTEGER	start;			//Start of timer
	LARGE_INTEGER	frequency;		//Ticks Per Second

	float lastTime;					//Last time GetTimedMS was called
};

#elif PSTATION4

#include <texture_tool\raw\timer.h>

class GameTimer {
public:
	GameTimer(void);
	~GameTimer(void) {}

	//How many milliseconds have passed since the GameTimer was created
	float	GetMS() {
		return t.getMilliseconds();
	}

	//How many milliseconds have passed since GetTimedMS was last called
	float	GetTimedMS() {
		float a = GetMS();
		float b = a - lastTime;
		lastTime = a;
		return b;
	}

protected:
	float	start = 0;			//Start of timer
	float	frequency = 0;		//Ticks Per Second

	float lastTime = 0;					//Last time GetTimedMS was called
	sce::TextureTool::Raw::Timer  t;
};

#endif //WIN_OGL