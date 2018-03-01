/*
Class:OGLWindow
Author:Rich Davison
Description:Creates and handles the OGLWindow, including the initialisation of the mouse and keyboard.
*/
#pragma once
#pragma warning( disable : 4099 )

#include <string>

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>

#include "../nclgl/Graphics/Renderer/OpenGL/OGLRenderer.h"

#include "Keyboard.h"
#include "Mouse.h"
#include "GameTimerBase.h"

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define WINDOWCLASS "WindowClass"

#include <vector>


class OGLRenderer;
enum CursorStyle
{
	CURSOR_STYLE_DEFAULT = 0,
	CURSOR_STYLE_GRAB,
	CURSOR_STYLE_MAX
};

#include "WindowBase.h"

class OGLWindow  : public WindowBase
{
public:
	//static bool Initialise(std::string title = "OpenGL Framework", int sizeX = 800, int sizeY = 600, bool fullScreen = false);
	//static void Destroy();
	//static OGLWindow& GetWindow() { return *window; }


	void	SetWindowTitle(std::string title, ...);

	bool	UpdateWindow();

	void	SetRenderer(OGLRenderer* r);

	HWND&	GetHandle();

	bool	HasInitialised();

	void	LockMouseToWindow(bool lock);
	void	ShowOSPointer(bool show);

	//nclgl::Maths::Vector2	GetScreenSize() { return size; };

	/*static Keyboard*	GetKeyboard() { return keyboard; }
	static Mouse*		GetMouse() { return mouse; }*/

	//Mouse class stores relative position, and this returns exact position relative to the top left of the window
	//	returns true if the mouse is within the bounds of the window or false otherwise
	bool GetMouseScreenPos(nclgl::Maths::Vector2* out_pos);

	void SetCursorStyle(CursorStyle style);

	GameTimerBase*   GetTimer() { return timer; }

	std::vector<RAWINPUT*> keyboardEvents;
	std::vector<RAWINPUT*> mouseEvents;
protected:
	void	CheckMessages(MSG &msg);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HCURSOR cursor[CURSOR_STYLE_MAX];

	HWND			windowHandle;

	//static OGLWindow*		window;
	//static Keyboard*	keyboard;
	//static Mouse*		mouse;

	GameTimerBase*	timer;

	OGLRenderer*		renderer;

	bool				forceQuit;
	bool				init;
	bool				fullScreen;
	bool				lockMouse;
	bool				showMouse;

	nclgl::Maths::Vector2				position;
	//nclgl::Maths::Vector2				size;

	float				elapsedMS;

	bool				mouseLeftWindow;

public:
	OGLWindow(std::string title = "OpenGL Framework", int sizeX = 800, int sizeY = 600, bool fullScreen = false);
	~OGLWindow(void);
};