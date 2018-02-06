#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\Player.h>

class Pickup : public GameObject
{
public:
	Pickup();
	~Pickup();

	virtual void effect(Player* player) {};
};

