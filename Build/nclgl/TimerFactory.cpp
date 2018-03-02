#include "TimerFactory.h"
#include "OGLGameTimer.h"
#include "PS4GameTimer.h"

GameTimerBase * TimerFactory::MakeGameTimer()
{
#ifdef WIN_OGL
	return new OGLGameTimer();
#elif PSTATION4
	return new PS4GameTimer();
#endif
}

TimerFactory::TimerFactory()
{
}


TimerFactory::~TimerFactory()
{
}
