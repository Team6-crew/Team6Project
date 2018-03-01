#ifdef PSTATION4
#pragma once

#include "pad.h"
#include "InputBase.h"

class PS4Input : public InputBase
{
public:
	PS4Input();
	~PS4Input();

	void Poll();

	AXIS	GetAxis(unsigned int i);
	float	GetButton(unsigned int i);
	bool	GetButtonDown(unsigned int i);


protected:
	AXIS	axis[MAX_AXIS];
	float   buttons[MAX_BUTTONS];

protected:
	void InitController();

	int32_t padHandle;
	ScePadControllerInformation padInfo;
};

#endif