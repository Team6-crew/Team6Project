#pragma once

#include <string>

namespace nclgl
{
	namespace Maths
	{
		class Vector3;
	}
}

class AudioEngineBase
{
public:
	AudioEngineBase();
	virtual ~AudioEngineBase();

	// 2D Sounds
	virtual void SetBackgroundSound(const std::string& fileName) = 0;
	virtual void PlaySound2D(const std::string& fileName, bool looping) = 0;
	virtual void StopBackgroundSound() = 0;

	// 3D Sounds
	virtual void PlaySound3D(const std::string& fileName, 
							 const nclgl::Maths::Vector3& position) = 0;
	virtual void SetListenerPosition(const nclgl::Maths::Vector3& position, 
									 const nclgl::Maths::Vector3& facing) = 0;
	
	virtual void Update(float dt) = 0;

	// General Functions
	virtual void StopAllSounds() = 0;
	// A volume of 0.0 is minimum and 1.0 is maximum. Values outside this range are clamped
	virtual void SetVolume(float volume) = 0;
};

