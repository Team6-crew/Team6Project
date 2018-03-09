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
	virtual void Eff_Speed(Player* player) {};
	virtual void Eff_Paint(Player* player) {};
	virtual void Eff_Stun(Player* player) {};
	virtual void SoftEffect(PlayerSoftBody* player) {};
	virtual void SoftEff_Speed(PlayerSoftBody* player) {};
	virtual void SoftEff_Paint(PlayerSoftBody* player) {};
	virtual void SoftEff_Stun(PlayerSoftBody* player) {};


	
private:

};

