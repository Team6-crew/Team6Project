#pragma once

#include "../AudioEngineBase.h"

#include <string>

namespace irrklang
{
	class ISoundEngine;
	class ISound;
}

class WinAudio : public AudioEngineBase
{
public:
	WinAudio();
	~WinAudio();
	 
	void SetBackgroundSound(const std::string& fileName) override;
	void PlaySound2D(const std::string& fileName, bool looping) override;
	void StopBackgroundSound() override;
	void StopAllSounds() override;
protected:
	irrklang::ISoundEngine * soundEngine = nullptr;
	irrklang::ISound* background = nullptr;
};

