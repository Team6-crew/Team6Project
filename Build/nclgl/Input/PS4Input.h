#ifdef PSTATION4
#pragma once

enum Button {
	TRIANGLE = 0,
	CIRCLE,
	SQUARE,
	CROSS
};

enum Stick
{
	LEFT_STICK = 0,
	RIGHT_STICK
};

#include "pad.h"
#include "InputBase.h"

class PS4Input : public InputBase
{
public:
	PS4Input();
	~PS4Input();



	virtual void ResetInput()  override;

	virtual bool IsAction(InputToken action) override;

	virtual void Update() override;
protected:
	AXIS	axis[MAX_AXIS];
	float   buttons[MAX_BUTTONS];


	AXIS	GetAxis(Stick i);
	float	GetButton(Button i);
	bool	GetButtonDown(Button i);

protected:
	void InitController();

	int32_t padHandle;
	ScePadControllerInformation padInfo;
};

#endif