#include "State.h"

class RoamingState : State
{
public:
	virtual State enter(GameObject* owner);
	virtual State update(GameObject* owner, float dt);
	virtual State exit(GameObject* owner);

private:
};
#pragma once
