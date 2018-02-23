#pragma once
#include "Pickup.h"

class WeaponPickup: public Pickup
{
public:
	WeaponPickup();

	~WeaponPickup();

	virtual void effect(Player* player) {};
	virtual void shoot() {};
};

