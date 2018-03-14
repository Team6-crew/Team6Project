#ifdef  PSTATION4

#include "PS4GameTimer.h"

PS4GameTimer::PS4GameTimer(void){
	start = std::chrono::system_clock::now();
}

float PS4GameTimer::GetMS()
{
	//return 0.0f;
	auto duration = std::chrono::system_clock::now() - start;
	std::chrono::milliseconds t = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	return t.count();
}

float PS4GameTimer::GetTimedMS()
{
	float a = GetMS();
	float b = a - lastTime;
	lastTime = a;
	return b;
}


#endif //  PSTATION4