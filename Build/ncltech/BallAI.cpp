#include "BallAI.h"
#include "Player.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>


#include <algorithm> //used for remove

float maxVel = 20.0f;

using namespace std;

GameObject* BallAI::AIBall;
State* roamingState;

BallAI::BallAI()
{
	
	StateMachine * AIStateMachine;
	AIStateMachine = new StateMachine(AIBall);
	AIStateMachine->setDefaultState(roamingState);

	AIBall = CommonUtils::BuildSphereObject("ball",
		Vector3(20.0f, 2.0f, -20.0f),	//Position leading to 0.25 meter overlap on faces, and more on diagonals
		1.0f,				//Half dimensions
		true,									//Has Physics Object
		1.0f,									//Mass
		true,									//Has Collision Shape
		false,									//Dragable by the user
		CommonUtils::GenColor(1.f, 1.f));

	AIBall->setDynamic(true);
	std::ifstream myReadFile;
	myReadFile.open("../pos.txt");
	string line;

	float xQ;
	float yQ;
	float zQ;
	Vector3 pos;
	int nodes = 0;

	if (myReadFile.is_open())
	{
		while (!myReadFile.eof())
		{
			string vector3;
			getline(myReadFile, line);
			remove(line.begin(), line.end(), ' '); //remove any spaces
			getline(myReadFile, vector3, '(');
			string x;
			getline(myReadFile, x, ',');
			string y;
			getline(myReadFile, y, ',');
			string z;
			getline(myReadFile, z, ')');

			if (myReadFile.eof())
				break;

			std::string::size_type sz;

			xQ = std::stof(x, &sz);
			yQ = std::stof(y, &sz);
			zQ = std::stof(z, &sz);

			

			pos = (Vector3(xQ, yQ, zQ));
			setNodes(pos);
			++nodes;
			cout << "Node " << nodes << " added \n";
		}
		myReadFile.close();
	}

}


BallAI::~BallAI()
{

}


Vector3 BallAI::seek(Vector3 TargetPos)
{
	Vector3 DesVelo = ((TargetPos - AIBall->Physics()->GetPosition()).Normalise() * maxVel);

	return(DesVelo - AIBall->Physics()->GetLinearVelocity());
}

Vector3 BallAI::followPath()
{
	//following a path created by a collection of nodes. You can make your own path by pressing 1 when playing.
	//This will print a vector3 to screen and add it to a txt file. This file is read before run time
	//Give it a shot and add as many nodes as you would like

	std::vector<Vector3> nodesList = getNodes();
	goal = nodesList[CurrentNode];

	Vector3 ballPos = AIBall->Physics()->GetPosition();
	// Switch to length square
	float distanceToGoal = (ballPos - goal).Length();

	if (distanceToGoal <= 10)
	{
		cout << "Changing to Node" << CurrentNode << "\n";
		Vector3 Node = goal;
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
	AIBall->Physics()->SetForce(followPath());


}
