#include "PlayerSoftBody.h"
#include <algorithm>
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>
#include <nclgl\Launchpad.h>
#include <ncltech\StunProjectile.h>
#include "CommonMeshes.h"
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <ncltech\StunProjectile.h>
#include <ncltech\PaintProjectile.h>
#include <ncltech\SceneManager.h>
#include <nclgl\GameLogic.h>


PlayerSoftBody::PlayerSoftBody(const std::string& name,
	nclgl::Maths::Vector3& pos,
	float radius,
	float inverse_mass,
	nclgl::Maths::Vector4& color,
	int tg)
{
	speed = 20.0f;
	ball = new Softbody("ball",
		pos,	//Position leading to 0.25 meter overlap on faces, and more on diagonals
		radius,				//Half dimensions
		true,									//Has Physics Object
		inverse_mass,									//Mass
		true,									//Has Collision Shape
		false,									//Dragable by the user
		color);

	ball->buildsoftbody();

	colour = color;
	currentBuff = Tags::BNothing;
	buffTime = 0.0f;
	currentBuffTime = 0.0f;

	for (int i = 0; i < 182; ++i) {
		ball->softball[i]->physicsNode->SetOnCollisionCallback(
			std::bind(
				&PlayerSoftBody::collisionCallback,		// Function to call
				this,					// Constant parameter (in this case, as a member function, we need a 'this' parameter to know which class it is)
				std::placeholders::_1,
				std::placeholders::_2));
		if (tg == 1)
			ball->softball[i]->SetTag(TSoftPlayer2);
		else if (tg == 2)
			ball->softball[i]->SetTag(TSoftPlayer3);
		else if (tg == 3)
			ball->softball[i]->SetTag(TSoftPlayer4);
		else
			ball->softball[i]->SetTag(TSoftPlayer1);
	}
	tag = tg;

	body = CommonUtils::BuildCuboidObject("body",
		nclgl::Maths::Vector3(0.0f, 2.0f, 0.0f),	//Position leading to 0.25 meter overlap on faces, and more on diagonals
		nclgl::Maths::Vector3(0.5, 0.5, 0.5),				//Half dimensions
		false,									//Has Physics Object
		0.0f,									//Mass
		false,									//Has Collision Shape
		false,									//Dragable by the user
		CommonUtils::GenColor(0.45f, 0.5f));

	canpaint = true;
	stunEffect = true;
	sensitivity = 0.0f;
	equippedStunWeapon = NULL;
	equippedPaintWeapon = NULL;
	time = 0.0f;
	stunDuration = 0.0f;

	bodyRenderNode = (*body->Render()->GetChildIteratorStart());
	
	camera_transform = RenderNodeFactory::Instance()->MakeRenderNode();
	camera_transform->SetTransform(nclgl::Maths::Matrix4::Translation(nclgl::Maths::Vector3(0, 10, 25)));

	(*body->Render()->GetChildIteratorStart())->AddChild(camera_transform);
	(*body->Render()->GetChildIteratorStart())->SetMesh(NULL);

	tempPitch = -20.f;
	tempYaw = 0.0f;
	colour = color;
}


PlayerSoftBody::~PlayerSoftBody()
{
}

GameObject* PlayerSoftBody::getTop() {
	int k = 0;
	top = ball->softball[0];
	for (int i = 1; i < 182; ++i) {
		if (ball->softball[i]->Physics()->GetPosition().y > top->Physics()->GetPosition().y) {
			top = ball->softball[i];
			++k;
		}
	}

	if (k == 0)
		bottom = ball->softball[181];
	else if (k == 181)
		bottom = ball->softball[0];
	else if (((k % 18) > 9) || (k % 18) == 0)
		bottom = ball->softball[k - 9];
	else
		bottom = ball->softball[k + 9];

	return top;
}

GameObject* PlayerSoftBody::getBottom() {
	getTop();
	return bottom;
}

GameObject* PlayerSoftBody::getFront() {
	int k = 0;
	front = ball->softball[0];
	for (int i = 1; i < 182; ++i) {
		if (ball->softball[i]->Physics()->GetPosition().z < top->Physics()->GetPosition().z) {
			front = ball->softball[i];
			++k;
		}
	}

	if (k == 0)
		back = ball->softball[181];
	else if (k == 181)
		back = ball->softball[0];
	else if (((k % 18) > 9) || (k % 18) == 0)
		back = ball->softball[k - 9];
	else
		back = ball->softball[k + 9];

	return front;
}

GameObject* PlayerSoftBody::getBack() {
	getFront();
	return back;
}

void PlayerSoftBody::setControls(KeyboardKeys up, KeyboardKeys down, KeyboardKeys left, KeyboardKeys right, KeyboardKeys jump, KeyboardKeys shoot) {
	move_up = up;
	move_down = down;
	move_left = left;
	move_right = right;
	move_jump = jump;
	move_shoot = shoot;
}

void PlayerSoftBody::resetCamera(float dt) {
	if (sensitivity > 0) {
		sensitivity -= dt * 7;
		if (sensitivity < 0.0f) sensitivity = 0.0f;
	}
	else if (sensitivity < 0) {
		sensitivity += dt * 7;
		if (sensitivity > 0.0f) sensitivity = 0.0f;
	}
}

bool PlayerSoftBody::collisionCallback(PhysicsNode* thisNode, PhysicsNode* otherNode) {

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
		launchpad->SoftLaunch(this);
		canjump = false;
		return false;
	}
	else if (otherNode->GetParent()->HasTag(Tags::TPaintable)) {
		RenderNodeBase* otherRenderNode = (*otherNode->GetParent()->Render()->GetChildIteratorStart());
		nclgl::Maths::Vector4 col1 = otherRenderNode->GetColourFromPlayer();
		nclgl::Maths::Vector4 col2 = (*thisNode->GetParent()->Render()->GetChildIteratorStart())->GetColour();
		if (col1.x != col2.x || col1.y != col2.y || col1.z != col2.z) {
			// adding score for painting the obj
			int index = -1;
			for (int k = 0; k < GameLogic::Instance()->getNumSoftPlayers(); k++)
			{
				if (col2.z == GameLogic::Instance()->getSoftPlayer(k)->getColour().z) {
					index = k;
				}
			}
			if (GameLogic::Instance()->GetPlayerCapturedObject(otherNode->GetParent()) == -1) {
				GameLogic::Instance()->setPaintPerc(index, otherRenderNode->GetCost());
				GameLogic::Instance()->SetPlayerCapturedObject(otherNode->GetParent(), index);
				cout << otherRenderNode->GetCost();
			}
			else if (GameLogic::Instance()->GetPlayerCapturedObject(otherNode->GetParent()) != index) {
				GameLogic::Instance()->setPaintPerc(index, otherRenderNode->GetCost());
				GameLogic::Instance()->setPaintPerc(GameLogic::Instance()->GetPlayerCapturedObject(otherNode->GetParent()), -otherRenderNode->GetCost());
				
				GameLogic::Instance()->SetPlayerCapturedObject(otherNode->GetParent(), index);
			}
			otherRenderNode->SetColourFromPlayer((*thisNode->GetParent()->Render()->GetChildIteratorStart())->GetColour());
			otherRenderNode->SetBeingPainted(true);
			otherRenderNode->SetPaintPercentage(0.0f);
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"paintbomb.wav", false);
		}
	}
	else if (otherNode->GetParent()->HasTag(Tags::TWash)) {
		Washingzone* wash = (Washingzone*)otherNode->GetParent();
		wash->SoftEffect(this);
		return false;
	}
	else if (otherNode->GetParent()->HasTag(Tags::TGround) || otherNode->GetParent()->HasTag(Tags::TCubes) || otherNode->GetParent()->HasTag(Tags::TRotCubes))
	{
		canjump = true;
	}
	return true;
}

void PlayerSoftBody::unequipPaintWeapon() {
	if (equippedPaintWeapon) {
		(*body->Render()->GetChildIteratorStart())->RemoveChild(equippedPaintWeapon);
		delete equippedPaintWeapon;
		equippedPaintWeapon = NULL;
	}
}

void PlayerSoftBody::unequipStunWeapon() {
	if (equippedStunWeapon) {
		(*body->Render()->GetChildIteratorStart())->RemoveChild(equippedStunWeapon);
		delete equippedStunWeapon;
		equippedStunWeapon = NULL;
	}
}

void PlayerSoftBody::shoot() {
	if (equippedStunWeapon) {
		AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"shoot.wav", false);
		nclgl::Maths::Vector3 up = nclgl::Maths::Vector3(0, 1, 0);
		nclgl::Maths::Vector3 right = nclgl::Maths::Vector3::Cross(forward, up);
		nclgl::Maths::Vector3 pos = getTop()->Physics()->GetPosition() + nclgl::Maths::Vector3(0, 3, 0) - right * 1.5f - forward * 2.0f;
		StunProjectile* projectile = new StunProjectile("p", pos, 0.3f, true, 0.5f, true, colour);
		projectile->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(-forward.x * 40.0f, 0.f, -forward.z * 40.0f));
		SceneManager::Instance()->GetCurrentScene()->AddGameObject(projectile);
		PhysicsEngine::Instance()->DeleteAfter(projectile, 3.0f);
	}
	else if (equippedPaintWeapon) {
		AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"shoot.wav", false);
		nclgl::Maths::Vector3 up = nclgl::Maths::Vector3(0, 1, 0);
		nclgl::Maths::Vector3 right = nclgl::Maths::Vector3::Cross(forward, up);
		nclgl::Maths::Vector3 pos = getTop()->Physics()->GetPosition() + nclgl::Maths::Vector3(0, 3, 0) - right * 1.5f - forward * 2.0f;
		PaintProjectile* projectile = new PaintProjectile("p", pos, 0.3f, true, 0.5f, true, colour);
		projectile->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(-forward.x * 40.0f, 0.f, -forward.z * 40.0f));
		SceneManager::Instance()->GetCurrentScene()->AddGameObject(projectile);
		PhysicsEngine::Instance()->DeleteAfter(projectile, 3.0f);
	}
}

void PlayerSoftBody::equipStunWeapon(nclgl::Maths::Vector4 colour) {
	if (!equippedStunWeapon) {
		unequipPaintWeapon();
		equippedStunWeapon = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::StaticCube(), colour);
		equippedStunWeapon->SetTransform(nclgl::Maths::Matrix4::Scale(nclgl::Maths::Vector3(0.3f, 0.3f, 1.5f))*nclgl::Maths::Matrix4::Translation(nclgl::Maths::Vector3(5.0f, -8.0f, 0.0f)));

		(*body->Render()->GetChildIteratorStart())->AddChild(equippedStunWeapon);
	}
}

bool PlayerSoftBody::stun(float dt) {
	if (stunDuration > 0.0f) {
		stunEffect = true;
		time += dt;
		stunDuration -= dt;
		// CC
		for (int i = 0; i < 182; ++i)
			ball->softball[i]->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(0, 0, 0));
		if (stunDuration > 2.0f)
		{	// Shake
			camera->SetYaw(camera->GetYaw() + 360.0f * 0.0005f * ((rand() % 200 - 100.0f) / 100.0f));
			camera->SetPitch(camera->GetPitch() + 360.0f * 0.0005f * ((rand() % 200 - 100.0f) / 100.0f));
			return true;
		}
		else {
			camera->SetYaw(tempYaw);
			camera->SetPitch(tempPitch);
		}
		return true;
	}
	else {
		return false;
	}
}

void PlayerSoftBody::equipPaintWeapon(nclgl::Maths::Vector4 colour) {
	if (!equippedPaintWeapon) {
		unequipStunWeapon();
		equippedPaintWeapon = RenderNodeFactory::Instance()->MakeRenderNode(CommonMeshes::StaticCube(), colour);
		equippedPaintWeapon->SetTransform(nclgl::Maths::Matrix4::Scale(nclgl::Maths::Vector3(0.3f, 0.3f, 1.5f))*nclgl::Maths::Matrix4::Translation(nclgl::Maths::Vector3(5.0f, -8.0f, 0.0f)));

		(*body->Render()->GetChildIteratorStart())->AddChild(equippedPaintWeapon);
	}
}

void PlayerSoftBody::handleInput(float dt) {
	nclgl::Maths::Vector3 jump(0, 20, 0);
	float rotation = 0.0f;
	float yaw = camera->GetYaw();
	float pitch = camera->GetPitch();
	nclgl::Maths::Vector3 up = nclgl::Maths::Vector3(0, 1, 0);
	nclgl::Maths::Vector3 right = nclgl::Maths::Vector3::Cross(forward, up);

	getTop();
	getFront();
	if (GameLogic::Instance()->gameHasStarted()) {
		if (Window::GetKeyboard()->KeyDown(move_up))
		{
			if (forward.z > 0) {
				top->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, (ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, -5 - forward.z * speed));
				bottom->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, -(ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, -5 - forward.z * speed));
				front->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, -forward.y * speed, -speed - forward.z * speed));
				back->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, forward.y * speed, -speed - forward.z * speed));
			}
			else {
				top->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, (ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, 5 - forward.z * speed));
				bottom->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, -(ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, 5 - forward.z * speed));
				front->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, -forward.y * speed, speed - forward.z * speed));
				back->Physics()->SetForce(nclgl::Maths::Vector3(-forward.x * speed, forward.y * speed, speed - forward.z * speed));
			}
		}

		if (Window::GetKeyboard()->KeyDown(move_down))
		{
			if (forward.z > 0) {
				top->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, (ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, 5 + forward.z * speed));
				bottom->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, -(ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, 5 + forward.z * speed));
				front->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, -forward.y * speed, speed + forward.z * speed));
				back->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, forward.y * speed, speed + forward.z * speed));
			}
			else {
				top->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, (ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, -5 + forward.z * speed));
				bottom->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, -(ball->GetRadius() - (forward.z * speed) * (forward.z * speed)) / 5, -5 + forward.z * speed));
				front->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, -forward.y * speed, -speed + forward.z * speed));
				back->Physics()->SetForce(nclgl::Maths::Vector3(forward.x * speed, forward.y * speed, -speed + forward.z * speed));
			}
		}
		if (Window::GetKeyboard()->KeyDown(move_left))
		{
			for (int i = 0; i < 182; ++i)
			{
				getBall()->softball[i]->Physics()->SetForce(
					getBall()->softball[i]->Physics()->GetForce() + right * getBall()->softball[i]->Physics()->GetForce().Length() * 0.00005f);
			}
			increaseSensitivity(dt);
			camera->SetYaw(yaw + sensitivity);
		}

		else if (Window::GetKeyboard()->KeyDown(move_right))
		{
			for (int i = 0; i < 182; ++i)
			{
				getBall()->softball[i]->Physics()->SetForce(
					getBall()->softball[i]->Physics()->GetForce() - right * getBall()->softball[i]->Physics()->GetForce().Length() * 0.00005f);
			}
			decreaseSensitivity(dt);
			camera->SetYaw(yaw + sensitivity);
		}

		else {
			resetCamera(dt);
			camera->SetYaw(yaw + sensitivity);
		}

		if ((Window::GetKeyboard()->KeyTriggered(move_jump)))
		{
			if (canjump == true) {
				AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"jump2.wav", false);
				for (int i = 0; i < 182; ++i) {
					ball->softball[i]->Physics()->SetLinearVelocity(ball->softball[i]->Physics()->GetLinearVelocity() + jump);
				}
				canjump = false;
			}
		}

		if ((Window::GetKeyboard()->KeyTriggered(move_shoot)))
		{
			shoot();
		}
	}
}

void PlayerSoftBody::updateBuffTime(float dt) {
	if (currentBuff != BNothing) {
		currentBuffTime += dt; {
			if (currentBuffTime > buffTime) {
				currentBuff = Tags::BNothing;
				currentBuffTime = 0.0f;
				unequipStunWeapon();
				unequipPaintWeapon();
				speed = 20.0f;
			}
		}
	}
}

void PlayerSoftBody::setCurrentBuff(Tags tag) {
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

void PlayerSoftBody::move(float dt) {
	updateBuffTime(dt);
	if (!stun(dt)) {
		nclgl::Maths::Vector3 ball_pos = nclgl::Maths::Vector3((ball->softball[0]->Physics()->GetPosition().x + ball->softball[181]->Physics()->GetPosition().x) / 2,
			(ball->softball[0]->Physics()->GetPosition().y + ball->softball[181]->Physics()->GetPosition().y) / 2,
			(ball->softball[0]->Physics()->GetPosition().z + ball->softball[181]->Physics()->GetPosition().z) / 2);

		forward = (camera->GetPosition() - ball_pos).Normalise();

		nclgl::Maths::Matrix4 worldTr = bodyRenderNode->GetWorldTransform();
		worldTr.SetPositionVector(ball_pos + nclgl::Maths::Vector3(0, 2, 0));
		bodyRenderNode->SetTransform(worldTr);

		handleInput(dt);
		
		speedLimit();
		wallLimit();
		jumpSlow();
		ToInfinityAndNotQuiteBeyond();
		bodyRenderNode->SetTransform(bodyRenderNode->GetTransform()*nclgl::Maths::Matrix4::Rotation(sensitivity, nclgl::Maths::Vector3(0, 1, 0)));

		camera->SetPosition(camera_transform->GetWorldTransform().GetPositionVector());

		
	}
}

void PlayerSoftBody::speedLimit() {
	

	if (bottom->Physics()->GetLinearVelocity().z < -11) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetForce(nclgl::Maths::Vector3(0, 0, 0));
		}
	}

	else if (bottom->Physics()->GetLinearVelocity().z > 11) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetForce(nclgl::Maths::Vector3(0, 0, 0));
		}
	}

	else if (bottom->Physics()->GetLinearVelocity().x > 11) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetForce(nclgl::Maths::Vector3(0, 0, 0));
		}
	}

	else if (bottom->Physics()->GetLinearVelocity().x < -11) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetForce(nclgl::Maths::Vector3(0, 0, 0));
		}
	}
	else
		return;
}

void PlayerSoftBody::wallLimit() {
	if (WORLD_SIZE - front->Physics()->GetPosition().x < 4) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetLinearVelocity(
				getBall()->softball[i]->Physics()->GetLinearVelocity() * 0.95);
		}
	}
	else if (WORLD_SIZE + front->Physics()->GetPosition().x < 3) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetLinearVelocity(
				getBall()->softball[i]->Physics()->GetLinearVelocity() * 0.95);
		}
	}
	else if (WORLD_SIZE - front->Physics()->GetPosition().z < 5.5) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetLinearVelocity(
				getBall()->softball[i]->Physics()->GetLinearVelocity() * 0.95);
		}
	}
	else if (WORLD_SIZE + front->Physics()->GetPosition().z < 3) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetLinearVelocity(
				getBall()->softball[i]->Physics()->GetLinearVelocity() * 0.95);
		}
	}	
}

void PlayerSoftBody::jumpSlow() {
	if (bottom->Physics()->GetLinearVelocity().y < 0 && top->Physics()->GetLinearVelocity().y < 0) {
		for (int i = 0; i < 182; ++i) {
			getBall()->softball[i]->Physics()->SetLinearVelocity(getBall()->softball[i]->Physics()->GetLinearVelocity()*0.99);
		}
	}
}

void PlayerSoftBody::ToInfinityAndNotQuiteBeyond() {
	if (top->Physics()->GetPosition().y > 150.0f) {
		for (int i = 0; i < 182; ++i) {
			PhysicsNode* node = getBall()->softball[i]->Physics();
			node->SetLinearVelocity(nclgl::Maths::Vector3(0.0, -1.0f * node->GetLinearVelocity().y, 0.0));
			//getBall()->softball[i]->Physics()->SetPosition(nclgl::Maths::Vector3(0.0, 5.0, 0.0));
			
		}
	}
}