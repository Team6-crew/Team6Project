#pragma once
#include "nclgl\AI\RoamingState.h"
#include <nclgl\AI\BallAI.h>
#include <ncltech\Player.h>
#include <nclgl\AI\StateMachine.h>
#include <nclgl\AI\ChasingState.h>
#include <string>
#include <nclgl/GameLogic.h>
#include <random>
#include <time.h>

using std::string;

using namespace nclgl::Maths;

void RoamingState::enter(StateMachine * sOwner)
{
	
}
void RoamingState::update(StateMachine * sOwner)
{
	{
		sOwner->getOwner()->Physics()->SetForce(RoamingState::followPath(sOwner));
	}
}

nclgl::Maths::Vector3 RoamingState::followPath(StateMachine* sOwner)
{
	BallAI * owner = dynamic_cast<BallAI*>(sOwner->getOwner());
	if (owner->getNodeList().empty())
	{
		owner->getNodeList();
	}
	
	int numOfPlayers = GameLogic::Instance()->getNumPlayers();
	float closestPlayer = 100000000.0f;
	nclgl::Maths::Vector3 AIBallPos = sOwner->getOwner()->Physics()->GetPosition();
	
	for (int i = 0; i < numOfPlayers; i++)
	{
		nclgl::Maths::Vector3 playerBallPos = GameLogic::Instance()->getPlayer(i)->Physics()->GetPosition();

		float distanceToPlayer = (playerBallPos - AIBallPos).Length();

		if (distanceToPlayer < closestPlayer)
		{
			closestPlayer = distanceToPlayer;
		}
	}
		// Switch to length square
		nclgl::Maths::Vector3 goal = owner->getNode(owner->getCurrentNode());
		if (closestPlayer <= 5)
		{
			sOwner->setCurrentState(sOwner, ChasingState::GetInstance());
		}
		if (closestPlayer >= 5.1)
		{
			float distanceToGoal = (AIBallPos - goal).Length();
			if (distanceToGoal <= 5)
			{
				std::cout << "Changing to Node" << owner->getCurrentNode()<< "\n";
				owner->increaseCurrentNode();
			}
		}
		return seek(sOwner, goal);
}

nclgl::Maths::Vector3 RoamingState::seek(StateMachine* sOwner, nclgl::Maths::Vector3 TargetPos)
{
	nclgl::Maths::Vector3 DesVelo = ((TargetPos - sOwner->getOwner()->Physics()->GetPosition()).Normalise() * maxVel);

	return(DesVelo - sOwner->getOwner()->Physics()->GetLinearVelocity());
}