#ifdef PSTATION4
#pragma once

#include <nclgl\GameTimerBase.h>

#include <chrono>


class PS4GameTimer: public GameTimerBase {
public:
	PS4GameTimer(void);
	~PS4GameTimer(void) {}

	//How many milliseconds have passed since the GameTimer was created
	float	GetMS();

	//How many milliseconds have passed since GetTimedMS was last called
	float	GetTimedMS();

protected:
	std::chrono::time_point<std::chrono::system_clock> 	start;			//Start of timer
	std::chrono::time_point<std::chrono::system_clock> 	frequency ;		//Ticks Per Second

	float lastTime = 0;					//Last time GetTimedMS was called
	
};

#endif