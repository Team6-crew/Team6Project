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
	// A volume of 0.0 is minimum and 1.0 is maximum. Values outside this range are clamped
	void SetVolume(float volume) override;

protected:
	irrklang::ISoundEngine * soundEngine = nullptr;
	irrklang::ISound* background = nullptr;
};

