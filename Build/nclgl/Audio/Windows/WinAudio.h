#pragma once

#include "../AudioEngineBase.h"

#include <string>

namespace irrklang
{
	class ISoundEngine;
}

class WinAudio : public AudioEngineBase
{
public:
	WinAudio();
	~WinAudio();

	void PlaySound2D(const std::string& fileName, bool looping) override;
protected:
	irrklang::ISoundEngine * soundEngine;
};

