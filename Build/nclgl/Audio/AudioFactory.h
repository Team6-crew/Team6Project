#pragma once

class AudioEngineBase;

#include <nclgl\TSingleton.h>


class AudioFactory : public TSingleton<AudioFactory>
{
	friend class TSingleton<AudioFactory>;

public:
	AudioEngineBase*	 MakeAudioEngine();
private:
	AudioFactory();
	~AudioFactory();
};

