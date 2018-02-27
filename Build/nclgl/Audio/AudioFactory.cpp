#include "AudioFactory.h"


#include "Windows\WinAudio.h"

AudioFactory::AudioFactory()
{
}


AudioFactory::~AudioFactory()
{
}

//AudioEngineBase*	AudioFactory::MakeAudioEngine()
//{
//#ifdef WIN_OGL
//	return new WinAudio();
//#endif // WIN_OGL
//
//}

AudioEngineBase*	AudioFactory::GetAudioEngine()
{
#ifdef WIN_OGL
	static AudioEngineBase* engine = new WinAudio();
	return engine;
#endif // WIN_OGL
}