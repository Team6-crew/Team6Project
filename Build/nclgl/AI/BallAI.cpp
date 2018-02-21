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
#include <nclgl\CollectandUsePickupState.h>
float maxVel = 5.0f;

using namespace std;

GameObject* BallAI::AIBall;

State* roamingState;


using namespace nclgl::Maths;

BallAI::BallAI()
{
	
	nclgl::Maths::Vector3 location = nclgl::Maths::Vector3(20.0f, 2.0f, -20.0f);
	nclgl::Maths::Vector4 colour = nclgl::Maths::Vector4(255.0f,255.0f, 255.0f, 1.0f);

	AIBall = CommonUtils::BuildSphereObject("AIball",
		location,	//Position leading to 0.25 meter overlap on faces, and more on diagonals
		1.0f,				//Half dimensions
		true,									//Has Physics Object
		1.0f,									//Mass
		true,									//Has Collision Shape
		false,									//Dragable by the user
		colour);

	AIBall->setName("Test");
	AIBall->setLocation(location);
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
