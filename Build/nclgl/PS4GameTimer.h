#ifdef PSTATION4

#include "GameTimerBase.h"
#pragma once

#include <texture_tool\raw\timer.h>

class PS4GameTimer: public GameTimerBase {
public:
	PS4GameTimer(void);
	~PS4GameTimer(void) {}

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
	float	start;			//Start of timer
	float	frequency ;		//Ticks Per Second

	float lastTime = 0;					//Last time GetTimedMS was called
	sce::TextureTool::Raw::Timer  t;
};

#endif