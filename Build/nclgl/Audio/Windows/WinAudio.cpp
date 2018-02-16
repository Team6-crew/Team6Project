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
	soundEngine->play2D(fileName.c_str(), looping);
}
