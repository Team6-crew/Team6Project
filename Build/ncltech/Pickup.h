#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\Player.h>
#include <ncltech\PlayerSoftBody.h>

class Player;
class PlayerSoftBody;
class Pickup : public GameObject
{
public:
	Pickup();
	~Pickup();

	virtual void Effect(Player* player) {};
	virtual void Effect(PlayerSoftBody* player) {};



	
private:

};

