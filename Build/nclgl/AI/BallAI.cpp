#pragma once
#include "BallAI.h"
#include "ncltech\Player.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <nclgl\AI\StateMachine.h>
#include <nclgl\GameLogic.h>
#include <ncltech\CommonMeshes.h>

#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <algorithm> //used for remove
#include "RoamingState.h"
#include <nclgl\CollectandUsePickupState.h>

float maxVel = 5.0f;

using namespace std;

State* roamingState;


using namespace nclgl::Maths;

BallAI::BallAI(const std::string& name,
	const Vector3& pos,
	float radius,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const Vector4& color)
{
	PhysicsNode* pnode = NULL;
	//Due to the way SceneNode/RenderNode's were setup, we have to make a dummy node which has the mesh and scaling transform
	// and a parent node that will contain the world transform/physics transform
	RenderNodeBase* rnode = RenderNodeFactory::Instance()->MakeRenderNode();

	RenderNodeBase* dummy = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::Sphere(), color);
	dummy->SetTransform(Matrix4::Scale(Vector3(radius, radius, radius)));
	rnode->AddChild(dummy);

	if (physics_enabled)
	{
		pnode = new PhysicsNode();
		pnode->SetPosition(pos);
		pnode->SetInverseMass(inverse_mass);
		pnode->SetColRadius(radius);
		pnode->SetElasticity(0.2f);

		if (!collidable)
		{
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
			pnode->SetInverseInertia(SphereCollisionShape(radius).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new SphereCollisionShape(radius);
			pnode->SetCollisionShape(pColshape);
			pnode->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}
	}

	tag = Tags::TPlayer;

	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(radius);

	friendlyName = name;
	renderNode = rnode;
	physicsNode = pnode;

	

	physicsNode->SetOnCollisionCallback(
		std::bind(
			&BallAI::collisionCallback,		// Function to call
			this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
			std::placeholders::_1,
			std::placeholders::_2)			// Variable parameter(s) that will be set by the callback function
	);
	setDynamic(true);

	RegisterPhysicsToRenderTransformCallback();

}


BallAI::~BallAI()
{
}

void BallAI::addBallAIPlayers()
{
	nclgl::Maths::Vector4 colours[4];
	colours[0] = nclgl::Maths::Vector4(1.0f, 0.0f, 0.69f, 1.0f);
	colours[1] = nclgl::Maths::Vector4(0.3f, 1.0f, 1.0f, 1.0f);
	colours[2] = nclgl::Maths::Vector4(1.0f, 0.68f, 0.33f, 1.0f);
	colours[3] = nclgl::Maths::Vector4(0.0f, 1.0f, 0.02f, 1.0f);

	nclgl::Maths::Vector4 colour = nclgl::Maths::Vector4(255.0f, 255.0f, 255.0f, 1.0f);
	nclgl::Maths::Vector3 location = nclgl::Maths::Vector3(20.0f, 2.0f, -20.0f);

	int num = GameLogic::Instance()->getnumAI();

	for (int i = 0; i < num; i++)
	{
		BallAI * AIBall = new BallAI("AIPlayer"+(i),
			nclgl::Maths::Vector3(50.0f*(i + 2), 70.f, 60.0f*(i + 1)),
			1.0f,
			true,
			1.0f,
			true,
			colours[i + GameLogic::Instance()->getNumPlayers()]);
		AIBall->SetPhysics(AIBall->Physics());

		GameLogic::Instance()->addAIPlayer(AIBall);
	
		AIBall->AIStateMachine = new StateMachine(AIBall);
		AIBall->AIStateMachine->setCurrentState(AIBall->AIStateMachine, RoamingState::GetInstance());
		AIBall->setStateMachine(AIBall->AIStateMachine);

		//	MapNavigation *mp = new MapNavigation();
		//AIBall->setMapNavigation(mp);

		cout << "AI Player " << i << " created \n";

		{
			int numOfPlayers = GameLogic::Instance()->getNumPlayers() + GameLogic::Instance()->getNumAIPlayers() + GameLogic::Instance()->getNumSoftPlayers();
			int AIChosenPath = rand() % numOfPlayers;
			std::string path = to_string(AIChosenPath);

			std::ifstream myReadFile;
			myReadFile.open("0pos.txt");
			//cout <<sOwner->getOwner->getName()<<" using " <<path << "pos.txt\n";
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
					AIBall->addNodesToList(pos);
					++nodeNum;
					std::cout << "Node " << nodeNum << " added \n";
				}
				myReadFile.close();
			}
		}

	}
}


void BallAI::move()
{	
	//BallAI::getMapNavigation()->usePath();
	BallAI::getStateMachine()->getCurrentState()->update(BallAI::getStateMachine());
	
}

bool BallAI::collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode) {
	if (otherNode->GetParent()->HasTag(Tags::TPickup)) {
		Pickup* pickup = (Pickup*)otherNode->GetParent();
		//pickup->effect(this);
		//PhysicsEngine::Instance()->DeleteNextFrame(pickup);
		return false;
	}
	else if (otherNode->GetParent()->HasTag(Tags::TGround))
	{
		
	}
	return true;
};
