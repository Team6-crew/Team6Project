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
#include <nclgl\Launchpad.h>

#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>

#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <algorithm> //used for remove
#include "RoamingState.h"
#include <nclgl\CollectandUsePickupState.h>
#include <ncltech\StunProjectile.h>
#include <ncltech\PaintProjectile.h>

float maxVel = 5.0f;

using namespace std;

State* roamingState;


using namespace nclgl::Maths;

DEFINE_HEAP(BallAI, "AI");

BallAI::BallAI(const std::string& name,
	const Vector3& pos,
	float radius,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	const Vector4& color)
{
	speed = 20.f;
	sensitivity = 0.0f;
	colour = color;
	currentBuff = Tags::BNothing;
	buffTime = 0.0f;
	currentBuffTime = 0.0f;

	PhysicsNode* pnode = NULL;
	//Due to the way SceneNode/RenderNode's were setup, we have to make a dummy node which has the mesh and scaling transform
	// and a parent node that will contain the world transform/physics transform
	RenderNodeBase* rnode = RenderNodeFactory::Instance()->MakeRenderNode();

	RenderNodeBase* dummy = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::Sphere(), color);
	dummy->SetTransform(nclgl::Maths::Matrix4::Scale(Vector3(radius, radius, radius)));
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

	tag = Tags::TAIPlayer;

	pnode->SetOnCollisionCallback(
		std::bind(
			&BallAI::collisionCallback,		// Function to call
			this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
			std::placeholders::_1,
			std::placeholders::_2));

	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(radius);

	friendlyName = name;
	renderNode = rnode;
	physicsNode = pnode;

	AIbody = CommonUtils::BuildCuboidObject("AIbody",
		nclgl::Maths::Vector3(0.0f, 2.0f, 0.0f),	//Position leading to 0.25 meter overlap on faces, and more on diagonals
		nclgl::Maths::Vector3(20.5,20.5, 20.5),				//Half dimensions
		false,									//Has Physics Object
		0.0f,									//Mass
		false,									//Has Collision Shape
		false,									//Dragable by the user
		CommonUtils::GenColor(0.45f, 0.5f));

	equippedStunWeapon = NULL;
	equippedPaintWeapon = NULL;
	time = 0.0f;
	stunDuration = 0.0f;

	bodyRenderNode = (*AIbody->Render()->GetChildIteratorStart());

	(*AIbody->Render()->GetChildIteratorStart())->SetMesh(NULL);

	setDynamic(true);

	RegisterPhysicsToRenderTransformCallback();

}


BallAI::~BallAI()
{
}

void BallAI::addBallAIPlayers(int i)
{
	nclgl::Maths::Vector4 colours[4];
	colours[0] = nclgl::Maths::Vector4(1.0f, 0.0f, 0.69f, 1.0f);
	colours[1] = nclgl::Maths::Vector4(0.3f, 1.0f, 1.0f, 1.0f);
	colours[2] = nclgl::Maths::Vector4(1.0f, 0.68f, 0.33f, 1.0f);
	colours[3] = nclgl::Maths::Vector4(0.0f, 1.0f, 0.02f, 1.0f);

	nclgl::Maths::Vector4 colour = nclgl::Maths::Vector4(255.0f, 255.0f, 255.0f, 1.0f);
	nclgl::Maths::Vector3 location = nclgl::Maths::Vector3(20.0f, 2.0f, -20.0f);

		BallAI * AIBall = new BallAI(" AIPlayer "+(i),
			nclgl::Maths::Vector3(3.0f*(i + 2), 7.f, 0.0f),
			1.0f,
			true,
			1.0f,
			true,
			colours[i]);
		AIBall->setColour(colours[i]);
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
			std::ifstream myReadFile;
			string file;
			if (i == 0)
				file = "0pos.txt";
			if (i == 1)
				file = "1pos.txt";
			if (i == 2)
				file = "2pos.txt";
			if (i == 3)
				file = "3pos.txt";

			myReadFile.open(file);

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


void BallAI::move(float dt)
{
	updateBuffTime(dt);
	if (!stun(dt))
	{
		nclgl::Maths::Vector3 ball_pos = physicsNode->GetPosition();
		nclgl::Maths::Matrix4 worldTr = bodyRenderNode->GetWorldTransform();
		worldTr.SetPositionVector(ball_pos + nclgl::Maths::Vector3(0, 2, 0));
		bodyRenderNode->SetTransform(worldTr);
		bodyRenderNode->SetTransform(bodyRenderNode->GetTransform());

		if (GameLogic::Instance()->gameHasStarted()) {
			getStateMachine()->getCurrentState()->update(getStateMachine());
			//BallAI::getMapNavigation()->usePath();
		}

		
	}
}

bool BallAI::collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode) {
	
	if (otherNode->GetParent()->HasTag(Tags::TWeapon)) {
		Pickup* pickup = (Pickup*)otherNode->GetParent();
		pickup->Effect(this);
		PhysicsEngine::Instance()->DeleteAfter(pickup, 0.0f);
		return false;
	}
	if (otherNode->GetParent()->HasTag(Tags::TRandomPickup))
	{
		Pickup* pickup = (Pickup*)otherNode->GetParent();
		pickup->Effect(this);
		PhysicsEngine::Instance()->DeleteAfter(pickup, 0.0f);
		return false;
	}

	else if (otherNode->GetParent()->HasTag(Tags::TLaunch))
	{
		AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"duang.wav", false);
		Launchpad* launchpad = (Launchpad*)otherNode->GetParent();
		//launchpad->Launch(this);
		return false;
	}
	else if (otherNode->GetParent()->HasTag(Tags::TPaintable)) {
		RenderNodeBase* otherRenderNode = (*otherNode->GetParent()->Render()->GetChildIteratorStart());
		Vector4 col1 = otherRenderNode->GetColourFromPlayer();
		Vector4 col2 = (*thisNode->GetParent()->Render()->GetChildIteratorStart())->GetColour();
		if (col1.x != col2.x || col1.y != col2.y || col1.z != col2.z) {
			otherRenderNode->SetColourFromPlayer((*thisNode->GetParent()->Render()->GetChildIteratorStart())->GetColour());
			otherRenderNode->SetBeingPainted(true);
			otherRenderNode->SetPaintPercentage(0.0f);
		}
	}
	if (otherNode->GetParent()->HasTag(Tags::TWash)) {
		Washingzone* wash = (Washingzone*)otherNode->GetParent();
		wash->AIEffect(this);
		return false;
	}

	return true;
};

void BallAI::unequipPaintWeapon() {
	if (equippedPaintWeapon) {
		(*AIbody->Render()->GetChildIteratorStart())->RemoveChild(equippedPaintWeapon);
		delete equippedPaintWeapon;
		equippedPaintWeapon = NULL;
	}
}

void BallAI::unequipStunWeapon() {
	if (equippedStunWeapon) {
		(*AIbody->Render()->GetChildIteratorStart())->RemoveChild(equippedStunWeapon);
		delete equippedStunWeapon;
		equippedStunWeapon = NULL;
	}
}

void BallAI::equipStunWeapon(Vector4 colour) {
	if (!equippedPaintWeapon) {
		unequipPaintWeapon();
		equippedStunWeapon = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::StaticCube(), colour);
		equippedStunWeapon->SetTransform(nclgl::Maths::Matrix4::Scale(nclgl::Maths::Vector3(0.3f, 0.3f, 1.5f))*nclgl::Maths::Matrix4::Translation(Vector3(5.0f, -8.0f, 0.0f)));

		(*AIbody->Render()->GetChildIteratorStart())->AddChild(equippedStunWeapon);
	}
}

void BallAI::equipPaintWeapon(Vector4 colour) {
	if (!equippedStunWeapon) {
		unequipStunWeapon();
		equippedPaintWeapon = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::StaticCube(), colour);
		equippedPaintWeapon->SetTransform(nclgl::Maths::Matrix4::Scale(nclgl::Maths::Vector3(0.3f, 0.3f, 1.5f))*nclgl::Maths::Matrix4::Translation(Vector3(5.0f, -8.0f, 0.0f)));

		(*AIbody->Render()->GetChildIteratorStart())->AddChild(equippedPaintWeapon);
	}
}

void BallAI::updateBuffTime(float dt) 
{
	float time = GameLogic::Instance()->getTotalTime();
	if (currentBuff != BNothing) {
		if (currentBuff == BPaint || BStun){
			int shootTime = 2;
			if ((((int)time % shootTime == 0)) && (canShoot))
			{
				shoot();

				canShoot = false;
			}

			if (((int)time % shootTime != 0))
			{
				canShoot = true;
			}
		}
		currentBuffTime += dt;
		 {
			if (currentBuffTime > buffTime) 
			{
				currentBuff = Tags::BNothing;
				currentBuffTime = 0.0f;
				unequipStunWeapon();
				unequipPaintWeapon();
				speed = 20.0f;
				getBall()->Physics()->SetInverseMass(1.0f);
			}
		}
	}
}

void BallAI::setCurrentBuff(Tags tag) {
	if (tag == BPaint) {
		unequipStunWeapon();
		speed = 20.0f;
	}
	else if (tag == BStun) {
		unequipPaintWeapon();
		speed = 20.0f;
	}
	else if (tag == BSpeed) {
		unequipStunWeapon();
		unequipPaintWeapon();
	}
	currentBuff = tag;
}

void BallAI::shoot() {

	BallAI * owner = dynamic_cast<BallAI*>(AIStateMachine->getOwner()); // cannot get nodes list any other way
	nclgl::Maths::Vector3 goal = owner->getNode(owner->getCurrentNode());
	forward = goal - getBall()->Physics()->GetPosition();

	if (equippedStunWeapon) {
		AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"shoot.wav", false);
		nclgl::Maths::Vector3 up = nclgl::Maths::Vector3(0, 1, 0);
		nclgl::Maths::Vector3 right = nclgl::Maths::Vector3::Cross(forward, up);
		nclgl::Maths::Vector3 pos = getBall()->Physics()->GetPosition() + nclgl::Maths::Vector3(0, 3, 0) - right * 1.5f;
		StunProjectile* projectile = new StunProjectile("p", pos, 0.3f, true, 0.5f, true, colour);
		projectile->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(-forward.x * 4.0f, 0.f, -forward.z * 5.0f));
		SceneManager::Instance()->GetCurrentScene()->AddGameObject(projectile);
		PhysicsEngine::Instance()->DeleteAfter(projectile, 3.0f);
	}
	else if (equippedPaintWeapon) {
		AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"shoot.wav", false);
		nclgl::Maths::Vector3 up = nclgl::Maths::Vector3(0, 1, 0);
		nclgl::Maths::Vector3 right = nclgl::Maths::Vector3::Cross(forward, up);
		nclgl::Maths::Vector3 pos = getBall()->Physics()->GetPosition() + nclgl::Maths::Vector3(0, 3, 0) - right * 1.5f;
		PaintProjectile* projectile = new PaintProjectile("p", pos, 0.3f, true, 0.5f, true, colour);
		projectile->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(-forward.x * 4.0f, 0.f, -forward.z * 5.0f));
		SceneManager::Instance()->GetCurrentScene()->AddGameObject(projectile);
		PhysicsEngine::Instance()->DeleteAfter(projectile, 3.0f);
	}
}

bool BallAI::stun(float dt) {
	if (stunDuration > 0.0f) {
		stunEffect = true;
		time += dt;
		stunDuration -= dt;
		getBall()->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
		if(stunDuration > 10.0f)
		return true;
	}
	else {
		return false;
	}
}