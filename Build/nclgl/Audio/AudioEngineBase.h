#pragma once

#include <string>

class AudioEngineBase
{
public:
	AudioEngineBase();
	virtual ~AudioEngineBase();


	virtual void PlaySound2D(const std::string& fileName, bool looping) = 0;
};

