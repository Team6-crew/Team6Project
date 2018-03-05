#include "TimerFactory.h"
// Refactor using chrono
#include <nclgl\Graphics\Renderer\OpenGL\OGLGameTimer.h>
#include <nclgl\Graphics\Renderer\PS4\PS4GameTimer.h>

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
