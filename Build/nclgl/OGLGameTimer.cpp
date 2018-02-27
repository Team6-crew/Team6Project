#include "OGLGameTimer.h"



OGLGameTimer::OGLGameTimer()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
	QueryPerformanceCounter((LARGE_INTEGER *)&start);

	lastTime = GetMS();
}

/*
Returns the Milliseconds since timer was started
*/
float OGLGameTimer::GetMS() {
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return (float)((t.QuadPart - start .QuadPart) * 1000.0 / frequency.QuadPart);
}

float	 OGLGameTimer::GetTimedMS() {
	float a = GetMS();
	float b = a - lastTime;
	lastTime = a;
	return b;
}