#pragma once
#include "nclgl\AI\RoamingState.h"
#include <nclgl\AI\BallAI.h>
#include <ncltech\Player.h>
#include <nclgl\AI\StateMachine.h>
#include <nclgl\AI\ChasingState.h>
#include <string>

using std::string;

using namespace nclgl::Maths;


void RoamingState::enter(StateMachine * sOwner, GameObject * owner)
{
	{
		std::ifstream myReadFile;
		myReadFile.open("../pos.txt");
		string line;

		float xQ;
		float yQ;
		float zQ;
		nclgl::Maths::Vector3 pos;
		int nodeNum = 0;
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



				pos = (nclgl::Maths::Vector3(xQ, yQ, zQ));
				RoamingState::addNodesToList(pos);
				++nodeNum;
				std::cout << "Node " << nodeNum << " added \n";
			}
			myReadFile.close();
		}
	}
}
void RoamingState::update(StateMachine * sOwner, GameObject * owner)
{
	{
		owner->Physics()->SetForce(RoamingState::followPath(sOwner, owner));

	}
}

nclgl::Maths::Vector3 RoamingState::seek(GameObject* owner, nclgl::Maths::Vector3 TargetPos)
	{
		nclgl::Maths::Vector3 DesVelo = ((TargetPos - owner->Physics()->GetPosition()).Normalise() * maxVel);

		return(DesVelo - owner->Physics()->GetLinearVelocity());
	}

nclgl::Maths::Vector3 RoamingState::followPath(StateMachine* sOwner, GameObject* owner)
{
	nclgl::Maths::Vector3 ballPos = BallAI::getBall()->Physics()->GetPosition();
	nclgl::Maths::Vector3 playerPos = Player::getBall()->Physics()->GetPosition();
	// Switch to length square
	nclgl::Maths::Vector3 goal = nodesList[CurrentNode];
	float distanceToPlayer = (ballPos - playerPos).Length();

	if (distanceToPlayer <= 10)
	{
		sOwner->setCurrentState(sOwner, ChasingState::GetInstance());

	}
	if (distanceToPlayer >= 11)
	{
		vector<nclgl::Maths::Vector3> nodesList = getNodes();
		
		float distanceToGoal = (ballPos - goal).Length();
		if (distanceToGoal <= 20)
		{
			std::cout << "Changing to Node" << CurrentNode << "\n";
			nclgl::Maths::Vector3 Node = goal;
			++CurrentNode;
			nodesList.erase(nodesList.begin()); //delete the first node
			addNodesToList(Node); //adds it back to the list
		}
	}
	return seek(owner, goal);
}
