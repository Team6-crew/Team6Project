#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\Player.h>
#include <ncltech\PlayerSoftBody.h>
#include <nclgl\AI\BallAI.h>

class Player;
class PlayerSoftBody;
class BallAI;
class Pickup : public GameObject
{
public:
	Pickup();
	~Pickup();

	virtual void Effect(Player* player) {};
	virtual void Effect(PlayerSoftBody* player) {};
	virtual void Effect(BallAI* AIBall) {};



	
private:

};

