#pragma once
#include "ChasingState.h"
#include <nclgl\AI\BallAI.h>
#include <nclgl\AI\StateMachine.h>
#include <nclgl\AI\RoamingState.h>
#include <nclgl\Vector3.h>
using namespace nclgl::Maths;

void ChasingState::update(StateMachine * sOwner, GameObject * owner)
{
	nclgl::Maths::Vector3 ballPos = BallAI::getBall()->Physics()->GetPosition();
	nclgl::Maths::Vector3 playerPos = Player::getBall()->Physics()->GetPosition();
	// Switch to length square

	float distanceToPlayer = (ballPos - playerPos).Length();

	if (distanceToPlayer <= 10)
	{
		nclgl::Maths::Vector3 AIVelocity = owner->Physics()->GetLinearVelocity();
		nclgl::Maths::Vector3 ballPos = owner->Physics()->GetPosition();

		nclgl::Maths::Vector3 goal = Player::getBall()->Physics()->GetPosition();
		nclgl::Maths::Vector3 desVelo = ((goal - ballPos).Normalise()) * maxVel;

		nclgl::Maths::Vector3 Steering = desVelo - AIVelocity;
		Steering = Steering / owner->Physics()->GetInverseMass();

		owner->Physics()->SetForce(nclgl::Maths::Vector3(desVelo));
	}
	if (distanceToPlayer > 11)
	{
		sOwner->setCurrentState(sOwner, RoamingState::GetInstance());
	}
}

void ChasingState::exit(StateMachine * sOwner, GameObject * owner)
{
	cout << "leaving ChasingState \n";
}

void ChasingState::enter(StateMachine * sOwner, GameObject * owner) { std::cout << "Entered ChasingState \n"; }