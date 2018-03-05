#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\Player.h>

class Player;
class Pickup : public GameObject
{
public:
	Pickup();
	~Pickup();

	virtual void eff_speed(Player* player) {};
	virtual void eff_paint(Player* player) {};
	virtual void eff_stun(Player* player) {};
	
private:

};

