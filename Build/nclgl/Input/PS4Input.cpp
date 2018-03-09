#ifdef PSTATION4



#include "PS4Input.h"
#include <sys\_defines\_sce_ok.h>
#include <iostream>
#include <user_service\user_service_api.h>

PS4Input::PS4Input() {
	SceUserServiceInitializeParams params;
	memset((void*)&params, 0, sizeof(params));
	params.priority = SCE_KERNEL_PRIO_FIFO_DEFAULT;
	if (sceUserServiceInitialize(&params) != SCE_OK) {
		std::cout << "PS4Input: Failed to initialise User Service!" << std::endl;
	};

	scePadInit();
	InitController();
	ResetInput();
}

PS4Input::~PS4Input() {
	scePadClose(padHandle);

	sceUserServiceTerminate();
}

void PS4Input::InitController() {
	//Get the currently logged in player
	SceUserServiceUserId userId;
	int ret = sceUserServiceGetInitialUser(&userId);

	if (ret == SCE_OK) {
		padHandle = scePadOpen(userId, SCE_PAD_PORT_TYPE_STANDARD, 0, NULL);

		//Can get connectivity / deadzone info from this structure.
		int ret = scePadGetControllerInformation(padHandle, &padInfo);
		if (ret == SCE_OK) {
			//can get deadzone information etc through here
		}
	}
}

void PS4Input::Update() {
	ScePadData data;

	int ret = scePadReadState(padHandle, &data);

	if (ret == SCE_OK) {
		if (data.connected) {
			axis[0].x = (((data.leftStick.x / 255.0f) * 2) - 1.0f);
			axis[0].y = (((data.leftStick.y / 255.0f) * 2) - 1.0f);
			//std::cout << axis[0].x << " "  << axis[0].y  << std::endl;


			axis[1].x = (((data.rightStick.x / 255.0f) * 2) - 1.0f);
			axis[1].y = (((data.rightStick.y / 255.0f) * 2) - 1.0f);


			axis[2].x = 0.0f;
			axis[2].x += ((data.buttons & SCE_PAD_BUTTON_RIGHT) ? 1.0f : 0.0f);
			axis[2].x -= ((data.buttons & SCE_PAD_BUTTON_LEFT) ? 1.0f : 0.0f);

			axis[2].y = 0.0f;
			axis[2].y += ((data.buttons & SCE_PAD_BUTTON_UP) ? 1.0f : 0.0f);
			axis[2].y -= ((data.buttons & SCE_PAD_BUTTON_DOWN) ? 1.0f : 0.0f);

			buttons[0] = ((data.buttons & SCE_PAD_BUTTON_TRIANGLE) ? 1.0f : 0.0f);
			buttons[1] = ((data.buttons & SCE_PAD_BUTTON_CIRCLE) ? 1.0f : 0.0f);
			buttons[2] = ((data.buttons & SCE_PAD_BUTTON_CROSS) ? 1.0f : 0.0f);
			buttons[3] = ((data.buttons & SCE_PAD_BUTTON_SQUARE) ? 1.0f : 0.0f);

			buttons[4] = data.analogButtons.l2 / 255.0f;
			buttons[5] = data.analogButtons.r2 / 255.0f;

			buttons[6] = ((data.buttons & SCE_PAD_BUTTON_L1) ? 1.0f : 0.0f);
			buttons[7] = ((data.buttons & SCE_PAD_BUTTON_R1) ? 1.0f : 0.0f);

			buttons[8] = ((data.buttons & SCE_PAD_BUTTON_L3) ? 1.0f : 0.0f);
			buttons[9] = ((data.buttons & SCE_PAD_BUTTON_R3) ? 1.0f : 0.0f);
		}
	}
}


void PS4Input::ResetInput() {
	for (int i = 0; i < MAX_AXIS; ++i) {
		axis[i].x = 0.0f;
		axis[i].y = 0.0f;
	}
	for (int i = 0; i < MAX_BUTTONS; ++i) {
		buttons[i] = 0.0f;
	}
}

AXIS	PS4Input::GetAxis(Stick i) {
	if (i > MAX_AXIS) {
		return AXIS();
	}
	return axis[i];
}

float	PS4Input::GetButton(Button i) {
	if (i > MAX_BUTTONS) {
		return 0.0f;
	}
	return buttons[i];
}

bool	PS4Input::GetButtonDown(Button i) {
	if (i > MAX_BUTTONS) {
		return false;
	}
	return buttons[i] > 0.5f ? true : false;
}



bool PS4Input::IsAction(InputToken action)
{
	switch (action)
	{
	case ESCAPE:
		return GetButtonDown(CROSS);
		
		break;
	case FORWARD:
		return (GetAxis(LEFT_STICK).y  < -0.5);
		break;
	case BACKWARD:
		return (GetAxis(LEFT_STICK).y > 0.5);
		break;
	case LEFT:
		return (GetAxis(LEFT_STICK).x  > 0.5);
		break;
	case RIGHT:
		return (GetAxis(LEFT_STICK).x <  -0.5);
		break;
	case ACTION_A:
		return GetButtonDown(TRIANGLE);
		break;
	default:
		return false;
	}
}


#endif // PSTATION4
