#ifdef WIN_OGL

#include "OGLWindow.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "TimerFactory.h"


using namespace nclgl::Maths;
//

#include <nclgl\WindowFactory.h>

OGLWindow::OGLWindow(std::string title, int sizeX, int sizeY, bool fullScreen)
	:WindowBase(sizeX, sizeY)
{
	renderer = NULL;
	forceQuit = false;
	init = false;
	mouseLeftWindow = false;
	lockMouse = false;
	showMouse = true;

	this->fullScreen = fullScreen;


	fullScreen ? position.x = 0.0f : position.x = 100.0f;
	fullScreen ? position.y = 0.0f : position.y = 100.0f;

	HINSTANCE hInstance = GetModuleHandle(NULL);


	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	if (!GetClassInfoEx(hInstance, WINDOWCLASS, &windowClass)) {
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = (WNDPROC)WindowProc;
		windowClass.hInstance = hInstance;
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		windowClass.lpszClassName = WINDOWCLASS;

		if (!RegisterClassEx(&windowClass)) {
			std::cout << "Window::Window(): Failed to register class!" << std::endl;
			return;
		}
	}

	if (fullScreen) {
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared

		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = sizeX;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = sizeY;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = 32;					// Selected Bits Per Pixel
		dmScreenSettings.dmDisplayFrequency = 60;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			std::cout << "Window::Window(): Failed to switch to fullscreen!" << std::endl;
			return;
		}
	}

	windowHandle = CreateWindowEx(fullScreen ? WS_EX_TOPMOST : NULL,
		WINDOWCLASS,    // name of the window class
		title.c_str(),   // title of the window
		fullScreen ? WS_POPUP | WS_VISIBLE : WS_OVERLAPPEDWINDOW | WS_VISIBLE,//S_VISIBLE | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,    // window style
		(int)position.x,	// x-position of the window
		(int)position.y,	// y-position of the window
		(int)screenSize.x,		// width of the window
		(int)screenSize.y,		// height of the window
		NULL,				// No parent window!
		NULL,				// No Menus!
		hInstance,			// application handle
		NULL);				// No multiple windows!

	if (!windowHandle) {
		std::cout << "Window::Window(): Failed to create window!" << std::endl;
		return;
	}


	timer = TimerFactory::Instance()->MakeGameTimer();
	elapsedMS = timer->GetMS();

	LockMouseToWindow(lockMouse);
	ShowOSPointer(showMouse);

	init = true;
}


OGLWindow::~OGLWindow(void)
{

}

HWND& OGLWindow::GetHandle() {
	return windowHandle;
}

bool OGLWindow::HasInitialised() {
	return init;
}

void OGLWindow::SetWindowTitle(std::string title, ...)
{
	va_list args;
	va_start(args, title);

	char buf[1024];
	memset(buf, 0, 1024);

	vsnprintf_s(buf, 1023, _TRUNCATE, title.c_str(), args);
	va_end(args);

	SetWindowTextA(windowHandle, buf);
}

bool OGLWindow::GetMouseScreenPos(Vector2* out_pos)
{
	Vector2(-1.0f, -1.0f);

	POINT p;
	if (GetCursorPos(&p))
	{
		if (ScreenToClient(windowHandle, &p))
		{
			Vector2 sp = Vector2((float)p.x, (float)p.y);
			if (sp.x >= 0.f && sp.y >= 0.f &&
				sp.x <= screenSize.x && sp.y <= screenSize.y)
			{
				if (out_pos) *out_pos = sp;
				return true;
			}
		}
	}

	return false;
}

void OGLWindow::SetCursorStyle(CursorStyle style)
{
	SetCursor(cursor[style]);
}

void	OGLWindow::SetRenderer(OGLRenderer* r) {
	renderer = r;
	if (r) {
		renderer->Resize((int)screenSize.x, (int)screenSize.y);
	}
}

bool	OGLWindow::UpdateWindow() {
	for (auto& event : keyboardEvents)
	{
		delete event;
	}
	keyboardEvents.clear();
	for (auto& event : mouseEvents)
	{
		delete event;
	}
	mouseEvents.clear();

	float diff = timer->GetMS() - elapsedMS;

	MSG		msg;
	while (PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE)) {
		CheckMessages(msg);
	}

	elapsedMS = timer->GetMS();

	return !forceQuit;
}



void OGLWindow::CheckMessages(MSG &msg) {

	switch (msg.message) {	// Is There A Message Waiting?		
	case (WM_QUIT):
	case (WM_CLOSE): {					// Have We Received A Quit Message?
		forceQuit = true;
	}break;
	case (WM_INPUT): {
		UINT dwSize;
		GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

		BYTE* lpb = new BYTE[dwSize];

		GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEKEYBOARD) {
			keyboardEvents.push_back(raw);
		}

		if (raw->header.dwType == RIM_TYPEMOUSE) {
			mouseEvents.push_back(raw);
		}
		
	}break;

	default: {								// If Not, Deal With OGLWindow Messages
		TranslateMessage(&msg);				// Translate The Message
		DispatchMessage(&msg);				// Dispatch The Message
	}
	}
}

LRESULT CALLBACK OGLWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case (WM_CREATE): {
		//Load our cursors
	} break;
	case WM_SETCURSOR: {
		return TRUE;
	} break;
	case(WM_DESTROY): {
	} break;
	case (WM_ACTIVATE): {
		return 0;
	}break;
	case (WM_LBUTTONDOWN): {

	}break;
	case (WM_MOUSEMOVE): {
	}break;
	case(WM_MOUSELEAVE): {
	}break;
	case(WM_SIZE): {
	}break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void	OGLWindow::LockMouseToWindow(bool lock) {
}

void	OGLWindow::ShowOSPointer(bool show) {
}

#endif