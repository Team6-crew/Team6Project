#pragma once

#include <string>

class AudioEngineBase
{
public:
	AudioEngineBase();
	virtual ~AudioEngineBase();

	virtual void SetBackgroundSound(const std::string& fileName) = 0;
	virtual void PlaySound2D(const std::string& fileName, bool looping) = 0;
	virtual void StopBackgroundSound() = 0;
	virtual void StopAllSounds() = 0;
};

