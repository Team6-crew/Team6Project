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

	AIBall->setDynamic(true);
	AIStateMachine = new StateMachine(AIBall);
	AIStateMachine->setCurrentState(AIStateMachine,RoamingState::GetInstance());
}


BallAI::~BallAI()
{
	delete AIStateMachine;
}


Vector3 BallAI::seek(nclgl::Maths::Vector3 TargetPos)
{
	nclgl::Maths::Vector3 DesVelo = ((TargetPos - AIBall->Physics()->GetPosition()).Normalise() * maxVel);

	return(DesVelo - AIBall->Physics()->GetLinearVelocity());
}

Vector3 BallAI::followPath()
{
	//following a path created by a collection of nodes. You can make your own path by pressing 1 when playing.
	//This will print a vector3 to screen and add it to a txt file. This file is read before run time
	//Give it a shot and add as many nodes as you would like
	std::vector<nclgl::Maths::Vector3> nodesList = getNodes();
	goal = nodesList[CurrentNode];

	nclgl::Maths::Vector3 ballPos = AIBall->Physics()->GetPosition();
	// Switch to length square
	float distanceToGoal = (ballPos - goal).Length();

	if (distanceToGoal <= 10)
	{
		cout << "Changing to Node " << CurrentNode << " at " << goal << "\n";
		nclgl::Maths::Vector3 Node = goal;
		CurrentNode++;
		nodesList.erase(nodesList.begin()); //delete the first node
		setNodes(Node); //adds it back to the list
	}
		return seek(goal);
	

}

void BallAI::chasePlayer()
{
	//currently a chasing behaviour
	//once all behaviours are done, will be added to a state machine

	Vector3 AIVelocity = AIBall->Physics()->GetLinearVelocity();
	Vector3 ballPos = AIBall->Physics()->GetPosition();

	Vector3 goal = Player::getBall()->Physics()->GetPosition();
	Vector3 desVelo = ((goal - ballPos).Normalise()) * maxVel;

	Vector3 Steering = desVelo - AIVelocity;
	Steering = Steering / AIBall->Physics()->GetInverseMass();

	AIBall->Physics()->SetForce(Vector3(desVelo));
}

void BallAI::move()
{
	AIStateMachine->getCurrentState()->update(AIStateMachine,AIBall);
//	AIBall->Physics()->SetForce(followPath());


}
