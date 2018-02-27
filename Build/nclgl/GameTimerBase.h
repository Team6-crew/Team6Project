#pragma once
class GameTimerBase
{
public:
	GameTimerBase(void) {};
	~GameTimerBase(void) {}

	//How many milliseconds have passed since the GameTimer was created
	virtual float	GetMS() = 0;

	//How many milliseconds have passed since GetTimedMS was last called
	virtual	float	GetTimedMS() = 0;

protected:
	
	float lastTime;					//Last time GetTimedMS was called
};

