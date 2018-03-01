#pragma once

#include <nclgl\TSingleton.h>

class WindowBase;

class WindowFactory : public TSingleton<WindowFactory>
{
	friend class TSingleton<WindowFactory>;
public:
	WindowBase * MakeWindow(std::string title = "OpenGL Framework", int sizeX = 800, int sizeY = 600, bool fullScreen = false);
	WindowBase * GetWindow();
private:
	WindowFactory();
	~WindowFactory();

	WindowBase* window;
};

