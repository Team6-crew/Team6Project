
#pragma once
#include <enet\enet.h>
#include <ncltech/Scene.h>
#include <ncltech/NetworkBase.h>


#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <nclgl\NCLDebug.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\Player.h>
#include <nclgl\Launchpad.h>
#include <nclgl\Portal.h>
#include <ncltech\Tags.h>
#include <ncltech\SpeedPickup.h>
#include <ncltech\WeaponPickup.h>
#include <ncltech\StunWeaponPickup.h>
#include <ncltech\Paintbomb.h>
#include <ncltech\WorldPartition.h>
#include <algorithm>
#include <nclgl/GameLogic.h>


//Basic Network Example

class Net1_Client : public Scene
{
public:
	Net1_Client(const std::string& friendly_name);

	virtual void OnInitializeScene() override;
	virtual void OnCleanupScene() override;
	virtual void OnUpdateScene(float dt) override;


	void ProcessNetworkEvent(const ENetEvent& evnt);

protected:
	GameObject * box;
	float rotation = 0.0f;
	NetworkBase network;
	ENetPeer*	serverConnection;
};