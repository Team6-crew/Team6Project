#pragma once

#include <nclgl/TSingleton.h>

#include <vector>

class GameTimerBase;


class TimerFactory : public TSingleton<TimerFactory>{

	friend class TSingleton<TimerFactory>;
public:
	GameTimerBase * MakeGameTimer();

private:
	TimerFactory();
	~TimerFactory();
};

