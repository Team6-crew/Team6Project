#pragma once
#include "ChasingState.h"
#include <nclgl\AI\BallAI.h>
#include <nclgl\AI\StateMachine.h>
#include <nclgl\AI\RoamingState.h>
#include <nclgl\Vector3.h>
#include <nclgl/GameLogic.h>
#include <limits>

using namespace nclgl::Maths;


void ChasingState::update(StateMachine * sOwner)
{
	BallAI * owner = dynamic_cast<BallAI*>(sOwner->getOwner());
	int numOfPlayers = GameLogic::Instance()->getNumPlayers();
	float closestPlayerDistance = 10000000000.0f;// std::numeric_limits<float>::max();
	int closestPlayer = 0; /// so we can get the correct player
	nclgl::Maths::Vector3 AIBallPos = owner->getBall()->Physics()->GetPosition();

	for (int i = 0; i < numOfPlayers; i++)
	{
		nclgl::Maths::Vector3 playerBallPos = GameLogic::Instance()->getPlayer(i)->Physics()->GetPosition();

		float distanceToPlayer = (playerBallPos - AIBallPos).Length();

		if (distanceToPlayer < closestPlayerDistance)
		{
			closestPlayerDistance = distanceToPlayer;
			if (closestPlayer == 0)
			closestPlayer = 0;
			else closestPlayer = i;
		}

	}
	// Switch to length square

	
	float distanceToPlayer = (AIBallPos - GameLogic::Instance()->getPlayer(closestPlayer)->Physics()->GetPosition()).Length();

	 if (distanceToPlayer <= 10)
	{
		 nclgl::Maths::Vector3 AIVelocity = sOwner->getOwner()->Physics()->GetLinearVelocity();
		 nclgl::Maths::Vector3 AIBallPos = sOwner->getOwner()->Physics()->GetPosition();

		 nclgl::Maths::Vector3 goal = GameLogic::Instance()->getPlayer(closestPlayer)->Physics()->GetPosition();
		 nclgl::Maths::Vector3 desVelo = ((goal - AIBallPos).Normalise()) * maxVel;

		 nclgl::Maths::Vector3 Steering = desVelo - AIVelocity;
		 Steering = Steering / sOwner->getOwner()->Physics()->GetInverseMass();

		 sOwner->getOwner()->Physics()->SetForce(nclgl::Maths::Vector3(desVelo));
	}
	 if (distanceToPlayer > 11)
	{
		sOwner->setCurrentState(sOwner, RoamingState::GetInstance());
	}
}

void ChasingState::exit(StateMachine * sOwner)
{
	cout << "leaving ChasingState \n";
}

void ChasingState::enter(StateMachine * sOwner) { std::cout << "Entered ChasingState \n"; }