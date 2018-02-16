#include "WinAudio.h"

#include <Irrklang\include\irrKlang.h>
using namespace irrklang;

#include <iostream>

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
	if (background)
	{
		delete background;
	}
	if (soundEngine)
	{
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