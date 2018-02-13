#pragma once

#include <stdlib.h>
#include <iostream>
#include <ncltech\Player.h>
#include <ncltech\BallAI.h>


class State
{
public:
	virtual State enter(GameObject* owner);
	virtual State update(GameObject* owner, float dt);
	virtual State exit(GameObject* owner);

private:
};
