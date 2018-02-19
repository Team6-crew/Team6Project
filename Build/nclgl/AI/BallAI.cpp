#pragma once
#include "BallAI.h"
#include "ncltech\Player.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <nclgl\AI\StateMachine.h>

#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <algorithm> //used for remove
#include "RoamingState.h"
float maxVel = 5.0f;

using namespace std;

GameObject* BallAI::AIBall;

State* roamingState;

using namespace nclgl::Maths;

BallAI::BallAI()
{
	


	AIBall = CommonUtils::BuildSphereObject("AIball",
		nclgl::Maths::Vector3(20.0f, 2.0f, -20.0f),	//Position leading to 0.25 meter overlap on faces, and more on diagonals
		1.0f,				//Half dimensions
		true,									//Has Physics Object
		1.0f,									//Mass
		true,									//Has Collision Shape
		false,									//Dragable by the user
		CommonUtils::GenColor(1.f, 1.f));

	AIBall->setName("Test");
	AIBall->setLocation(nclgl::Maths::Vector3(20.0f, 2.0f, -20.0));
	AIBall->setHalfDimentions(1.0f);
	AIBall->setHasPhysics(true);
	AIBall->setMass(1.1f);
	AIBall->setCollidable(true);
	AIBall->setDragable(false);
	AIBall->setDynamic(true);
	AIStateMachine = new StateMachine(AIBall);
	AIStateMachine->setCurrentState(AIStateMachine,RoamingState::GetInstance());
}


BallAI::~BallAI()
{
	delete AIStateMachine;
}


void BallAI::move()
{
	AIStateMachine->getCurrentState()->update(AIStateMachine,AIBall);

}
