#pragma once

#include <map>

enum InputToken
{
	FORWARD,
	LEFT,
	RIGHT,
	BACKWARD,
	ESCAPE,
	ACTION_A,
};


struct AXIS {
	float x;
	float y;

	AXIS() {
		x = 0.0f;
		y = 0.0f;
	}
};

#define MAX_AXIS 8
#define MAX_BUTTONS 127

class InputBase
{
public:
	InputBase();
	~InputBase();

	virtual void ResetInput() = 0;

	virtual bool IsAction(InputToken action) { return false; }

	virtual void Sleep() { isAwake = false; }
	virtual void Wake() { isAwake = true; }

	virtual void Update() = 0;

protected:
	bool isAwake;
};

