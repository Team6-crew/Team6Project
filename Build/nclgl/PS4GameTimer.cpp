#ifdef  PSTATION4

#include "PS4GameTimer.h"

PS4GameTimer::PS4GameTimer(void){
}

float PS4GameTimer::GetMS()
{
	return 0.0f;
	//return t.getMilliseconds();
}

float PS4GameTimer::GetTimedMS()
{
	float a = GetMS();
	float b = a - lastTime;
	lastTime = a;
	return b;
}


#endif //  PSTATION4