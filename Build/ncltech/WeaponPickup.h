#pragma once
#include "Pickup.h"

class WeaponPickup: public Pickup
{
public:
	WeaponPickup();

	~WeaponPickup();

	virtual void Effect(Player* player) {};
	virtual void SoftEffect(PlayerSoftBody* player) {};
	virtual void Shoot() {};
};

