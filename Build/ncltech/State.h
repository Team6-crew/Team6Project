#pragma once

#include <stdlib.h>
#include <iostream>




class State
{
public:
	virtual void enter(GameObject* owner) = 0;
	virtual void update(GameObject* owner) = 0;
	virtual void exit(GameObject* owner) = 0;


private:
};
