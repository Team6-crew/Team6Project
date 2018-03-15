#pragma once

class AudioEngineBase;

#include <nclgl\TSingleton.h>


class AudioFactory : public TSingleton<AudioFactory>
{
	friend class TSingleton<AudioFactory>;

public:
	// Don't want to make a new audio engine every time
	//AudioEngineBase*	 MakeAudioEngine();
	
	AudioEngineBase*	 GetAudioEngine();
private:
	AudioFactory();
	~AudioFactory();
};

