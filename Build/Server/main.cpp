
/******************************************************************************
Class: Net1_Client
Implements:
Author: Pieran Marris <p.marris@newcastle.ac.uk> and YOU!
Description:

:README:
- In order to run this demo, we also need to run "Tuts_Network_Client" at the same time.
- To do this:-
	1. right click on the entire solution (top of the solution exporerer) and go to properties
	2. Go to Common Properties -> Statup Project
	3. Select Multiple Statup Projects
	4. Select 'Start with Debugging' for both "Tuts_Network_Client" and "Tuts_Network_Server"

- Now when you run the program it will build and run both projects at the same time. =]
- You can also optionally spawn more instances by right clicking on the specific project
and going to Debug->Start New Instance.




FOR MORE NETWORKING INFORMATION SEE "Tuts_Network_Client -> Net1_Client.h"



		(\_/)
		( '_')
	 /""""""""""""\=========     -----D
	/"""""""""""""""""""""""\
....\_@____@____@____@____@_/

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <nclgl/MySocket.h>
#include <nclgl\GameTimer.h>
#include <nclgl\Vector3.h>
#include <nclgl\common.h>
#include <ncltech\NetworkBase.h>
#include <map>
#include <ncltech/Menu.h>
#include <ncltech/PhysicsEngine.h>
#include <ncltech/GraphicsPipeline.h>
#include <ncltech/SceneManager.h>
#include "../GameTech Coursework/EmptyScene.h"
//Needed to get computer adapter IPv4 addresses via windows
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
int bucket = 0;
class Menu;
PerfTimer timer_total, timer_physics, timer_update, timer_render;
int bin_players = 0;
bool gameStarted = false;
#define SERVER_PORT 1234
#define UPDATE_TIMESTEP (1.0f / 30.0f) //send 30 position updates per second
void eraseElement(ENetPeer* Item);
NetworkBase server;
GameTimer timer;
float accum_time = 0.0f;
float rotation = 0.0f;
vector <ENetPeer *> PlayerMap;
std::map <ENetPeer *, bool> ReadyMap;
void Win32_PrintAllAdapterIPAddresses();
void mainLoop();
int onExit(int exitcode)
{
	server.Release();
	system("pause");
	exit(exitcode);
}

int main(int arcg, char** argv)
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}

	//Initialize Server on Port 1234, with a possible 32 clients connected at any time
	if (!server.Initialize(SERVER_PORT, 32))
	{
		fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
		onExit(EXIT_FAILURE);
	}

	printf("Server Initiated\n");


	Win32_PrintAllAdapterIPAddresses();
	GraphicsPipeline::Instance();
	PhysicsEngine::Instance();
	SceneManager::Instance()->EnqueueScene(new EmptyScene("Lan Project"));
	timer.GetTimedMS();
	while (Window::GetWindow().UpdateWindow())
	{
		
		float dt = Window::GetWindow().GetTimer()->GetTimedMS() * 0.001f;	//How many milliseconds since last update?													
																			//Update Performance Timers (Show results every second)
		accum_time += dt;
		rotation += 0.5f * PI * dt;
		mainLoop();
		//Handle All Incoming Packets and Send any enqued packets
		server.ServiceNetwork(dt, [&](const ENetEvent& evnt)
		{
			if (evnt.type == ENET_EVENT_TYPE_CONNECT)
			{
				printf("- New Client Connected\n");
				MySocket LobbyConnection("LBCN");
				PlayerMap.push_back(evnt.peer);
				bin_players += pow(2, PlayerMap.size() - 1);
				ReadyMap[evnt.peer] = FALSE;
				LobbyConnection.AddVar(to_string(PlayerMap.size()-1));
				LobbyConnection.SendPacket(evnt.peer);
			}
			else if (evnt.type == ENET_EVENT_TYPE_RECEIVE){
				MySocket Received(evnt.packet);
				string SocketId = Received.GetPacketId();
				if (SocketId == "CNCN" || SocketId == "REDY") {
					if (SocketId == "REDY") ReadyMap[evnt.peer] = !ReadyMap[evnt.peer];
					int readys = 0;
					map <ENetPeer*, bool> ::iterator itr;
					for (itr = ReadyMap.begin(); itr != ReadyMap.end();++itr) {
						readys += itr->second;
					}
					MySocket PlayersConnected("PLCN");
					PlayersConnected.AddVar(to_string(readys));
					PlayersConnected.AddVar(to_string(PlayerMap.size()));
					PlayersConnected.BroadcastPacket(server.m_pNetwork);
					if (SocketId == "REDY" && PlayerMap.size()>1 && PlayerMap.size() == readys) {
						MySocket StartGame("STRT");
						gameStarted = true;
						StartGame.BroadcastPacket(server.m_pNetwork);
						int num_p = bin_players;
						if (num_p & 0b0001) GameLogic::Instance()->addPlayer(0);
						if (num_p & 0b0010) GameLogic::Instance()->addPlayer(1);
						if (num_p & 0b0100) GameLogic::Instance()->addPlayer(2);
						if (num_p & 0b1000) GameLogic::Instance()->addPlayer(3);
						//Add player to scene
						for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); i++) {
							SceneManager::Instance()->GetCurrentScene()->AddGameObject(GameLogic::Instance()->getPlayer(i));
							SceneManager::Instance()->GetCurrentScene()->AddGameObject(GameLogic::Instance()->getPlayer(i)->getBody());
						}
					}
				}
				if (SocketId == "INFO") {
					bucket++;
					for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); i++) {
						if (PlayerMap[i] == evnt.peer) {
							GameLogic::Instance()->getPlayer(i)->Physics()->SetLinearVelocity(nclgl::Maths::Vector3(stof(Received.TruncPacket(0)), stof(Received.TruncPacket(1)), stof(Received.TruncPacket(2))));
						}
					}
					// Here it applies all the variables to the appropriate balls
					if (bucket == PlayerMap.size()) {
						bucket = 0;
						MySocket NextPacket("NEXT");
						for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); i++) {
							NextPacket.AddVar(to_string(GameLogic::Instance()->getPlayer(i)->Physics()->GetLinearVelocity().x));
							NextPacket.AddVar(to_string(GameLogic::Instance()->getPlayer(i)->Physics()->GetLinearVelocity().y));
							NextPacket.AddVar(to_string(GameLogic::Instance()->getPlayer(i)->Physics()->GetLinearVelocity().z));
							NextPacket.AddVar(to_string(GameLogic::Instance()->getPlayer(i)->getRelativePosition().x));
							NextPacket.AddVar(to_string(GameLogic::Instance()->getPlayer(i)->getRelativePosition().y));
							NextPacket.AddVar(to_string(GameLogic::Instance()->getPlayer(i)->getRelativePosition().z));
						}
						NextPacket.BroadcastPacket(server.m_pNetwork);
					}
				}
				enet_packet_destroy(evnt.packet);
			}
			else if (ENET_EVENT_TYPE_DISCONNECT) {
				
				eraseElement(evnt.peer);
				MySocket PlayersConnected("PLCN");
				PlayersConnected.AddVar(to_string(PlayerMap.size()));
				PlayersConnected.BroadcastPacket(server.m_pNetwork);
				printf("- Client %d has disconnected.\n", evnt.peer->incomingPeerID);
			}
		});
		
		//Broadcast update packet to all connected clients at a rate of UPDATE_TIMESTEP updates per second
		if (accum_time >= UPDATE_TIMESTEP)
		{
		}

		Sleep(0);
	}

	system("pause");
	server.Release();
}

void eraseElement(ENetPeer* Item) {
	bool isFound = false;
	vector <ENetPeer*>::iterator it;
	for (it = PlayerMap.begin(); it != PlayerMap.end(); ++it) {
		if (*it == Item) {
			it = PlayerMap.erase(it); // After erasing, it3 is now pointing the next location.
			--it; // Go to the prev location because of ++it3 in the end of for loop.
			isFound = true;
		}
	}
}


//Yay Win32 code >.>
//  - Grabs a list of all network adapters on the computer and prints out all IPv4 addresses associated with them.
void Win32_PrintAllAdapterIPAddresses()
{
	//Initially allocate 5KB of memory to store all adapter info
	ULONG outBufLen = 5000;
	

	IP_ADAPTER_INFO* pAdapters = NULL;
	DWORD status = ERROR_BUFFER_OVERFLOW;

	//Keep attempting to fit all adapter info inside our buffer, allocating more memory if needed
	// Note: Will exit after 5 failed attempts, or not enough memory. Lets pray it never comes to this!
	for (int i = 0; i < 5 && (status == ERROR_BUFFER_OVERFLOW); i++)
	{
		pAdapters = (IP_ADAPTER_INFO *)malloc(outBufLen);
		if (pAdapters != NULL) {

			//Get Network Adapter Info
			status = GetAdaptersInfo(pAdapters, &outBufLen);

			// Increase memory pool if needed
			if (status == ERROR_BUFFER_OVERFLOW) {
				free(pAdapters);
				pAdapters = NULL;
			}
			else {
				break;
			}
		}
	}

	
	if (pAdapters != NULL)
	{
		//Iterate through all Network Adapters, and print all IPv4 addresses associated with them to the console
		// - Adapters here are stored as a linked list termenated with a NULL next-pointer
		IP_ADAPTER_INFO* cAdapter = &pAdapters[0];
		while (cAdapter != NULL)
		{
			IP_ADDR_STRING* cIpAddress = &cAdapter->IpAddressList;
			while (cIpAddress != NULL)
			{
				printf("\t - Listening for connections on %s:%u\n", cIpAddress->IpAddress.String, SERVER_PORT);
				cIpAddress = cIpAddress->Next;
			}
			cAdapter = cAdapter->Next;
		}

		free(pAdapters);
	}
	
}

void mainLoop() {
	float dt = Window::GetWindow().GetTimer()->GetTimedMS() * 0.001f;	//How many milliseconds since last update?
																		//Update Performance Timers (Show results every second)
	timer_total.UpdateRealElapsedTime(dt);
	timer_physics.UpdateRealElapsedTime(dt);
	timer_update.UpdateRealElapsedTime(dt);
	timer_render.UpdateRealElapsedTime(dt);
	//std::cout << "\nFPS: " + std::to_string(1000.f / timer_total.GetAvg());
	//Print Status Entries
	//PrintStatusEntries();


	timer_total.BeginTimingSection();

	//Update Scene
	timer_update.BeginTimingSection();
	SceneManager::Instance()->GetCurrentScene()->OnUpdateScene(dt);
	timer_update.EndTimingSection();

	//Update Physics	
	timer_physics.BeginTimingSection();
	PhysicsEngine::Instance()->Update(dt);
	timer_physics.EndTimingSection();
	PhysicsEngine::Instance()->DebugRender();

	//Render Scene
	timer_render.BeginTimingSection();
	GraphicsPipeline::Instance()->UpdateScene(dt);

	// Update Audio
	AudioFactory::Instance()->GetAudioEngine()->Update(dt);


	// Remove this once main menu is hooked up


	GraphicsPipeline::Instance()->RenderScene(dt);
	




	{
		//Forces synchronisation if vsync is disabled
		// - This is solely to allow accurate estimation of render time
		// - We should NEVER normally lock our render or game loop!		
		//	glClientWaitSync(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, NULL), GL_SYNC_FLUSH_COMMANDS_BIT, 1000000);
	}
	timer_render.EndTimingSection();



	//Finish Timing
	timer_total.EndTimingSection();
}