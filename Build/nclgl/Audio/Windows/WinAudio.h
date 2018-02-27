#pragma once

#include "../AudioEngineBase.h"

#include <string>

#include <nclgl\Vector3.h>

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

	void Update(float dt) override;

	void PlaySound3D(const std::string& fileName, 
					 const nclgl::Maths::Vector3& position) override;
	void SetListenerPosition(const nclgl::Maths::Vector3& position,
		const nclgl::Maths::Vector3& facing) override;

protected:
	irrklang::ISoundEngine * soundEngine = nullptr;
	irrklang::ISound* background = nullptr;
};

