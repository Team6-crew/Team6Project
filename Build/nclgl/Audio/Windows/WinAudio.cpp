#include "WinAudio.h"

#include <Irrklang\include\irrKlang.h>
using namespace irrklang;

#include <iostream>

#include <nclgl\common.h>
#include <ncltech\GraphicsPipeline.h>

using namespace nclgl::Maths;

WinAudio::WinAudio()
{
	soundEngine = createIrrKlangDevice();
	if (!soundEngine)
	{
		std::cout << "Error starting up Irrklang Engine" << std::endl;
		return;
	}
}


WinAudio::~WinAudio()
{
	soundEngine->drop();
}

void WinAudio::PlaySound2D(const std::string& fileName, bool looping)
{
	if (soundEngine)
	{
		soundEngine->play2D(fileName.c_str(), looping);
	}
	
}

void WinAudio::SetBackgroundSound(const std::string& fileName)
{

		if (soundEngine)
		{
			StopBackgroundSound();
			background = soundEngine->play2D(fileName.c_str(), true, false, true);
		}


}

void WinAudio::StopBackgroundSound()
{
	if (background)
	{
		background->stop();
	}

}

void WinAudio::StopAllSounds()
{
	if (soundEngine)
	{
		soundEngine->stopAllSounds();
	}

}


void WinAudio::SetVolume(float volume)
{
	volume = min(1.0, max(volume, 0.0f));
	if (soundEngine)
	{
		soundEngine->setSoundVolume(volume);
	}
}

void WinAudio::PlaySound3D(const std::string& fileName, const Vector3& position)
{
	if (soundEngine)
	{
		soundEngine->play3D(fileName.c_str(), vec3df(position.x, position.y, position.z));
	}
}

void WinAudio::SetListenerPosition(const Vector3& position, const Vector3& facing)
{
	if (soundEngine)
	{
		soundEngine->setListenerPosition(irrklang::vec3df(position.x, position.y, position.z),
			irrklang::vec3df(facing.x, facing.y, facing.z));
	}
}

void WinAudio::Update(float dt)
{
	SetListenerPosition(GraphicsPipeline::Instance()->GetCamera()->GetPosition(),
					   GraphicsPipeline::Instance()->GetCamera()->GetFacingDirection());
}