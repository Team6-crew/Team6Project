#pragma once
#define WIN32_LEAN_AND_MEAN
#include <ncltech\PhysicsEngine.h>
#include <ncltech\SceneManager.h>
#include <ncltech\NetworkBase.h>
#include <nclgl/MySocket.h>
#include <nclgl\NCLDebug.h>
#include <nclgl\PerfTimer.h>
#include <ncltech\OcTree.h>
#include "ServerScene.h"
#include "iostream"
#include "fstream"

#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>
#include <nclgl\ResourceManager.h>
#include <nclgl/LevelLoader.h>

#include <ncltech\Memory Management\HeapFactory.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
using namespace nclgl::Maths;
bool gameStarted = false;
int bucket = 0;
int bin_players = 0;
#define SERVER_PORT 1234
#define UPDATE_TIMESTEP (1.0f / 30.0f) //send 30 position updates per second
void eraseElement(ENetPeer* Item);
NetworkBase server;
GameTimer timer;
int fixTimer;
float accum_time = 0.0f;
float rotation = 0.0f;
vector <ENetPeer *> PlayerMap;
std::map <ENetPeer *, bool> ReadyMap;
const Vector4 status_colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 status_colour_header = Vector4(0.8f, 0.9f, 1.0f, 1.0f);

bool show_perf_metrics = false;
PerfTimer timer_total, timer_physics, timer_update, timer_render;
uint shadowCycleKey = 4;
void Win32_PrintAllAdapterIPAddresses();

// Program Deconstructor
//  - Releases all global components and memory
//  - Optionally prints out an error message and
//    stalls the runtime if requested.
void Quit(bool error = false, const std::string &reason = "") {
	//Release Singletons
	SceneManager::Release();
	PhysicsEngine::Release();
	GraphicsPipeline::Release();
	ResourceManager::Release();

	//Show console reason before exit
	if (error) {
		std::cout << reason << std::endl;
		system("PAUSE");
		exit(-1);
	}
}


// Program Initialise
//  - Generates all program wide components and enqueues all scenes
//    for the SceneManager to display
int Initialize()
{
	//Initialize Renderer
	GraphicsPipeline::Instance();

	//Initialise the PhysicsEngine
	PhysicsEngine::Instance();

	//Enqueue All Scenes
	SceneManager::Instance()->EnqueueScene(new ServerScene("Team Project"));
	Win32_PrintAllAdapterIPAddresses();
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}

	//Initialize Server on Port 1234, with a possible 32 clients connected at any time
	if (!server.Initialize(SERVER_PORT, 32))
	{
		fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
		//onExit(EXIT_FAILURE);
	}
	// Move this once main menu is hooked up
	//AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Intro.wav", false);
}

// Print Debug Info
//  - Prints a list of status entries to the top left
//    hand corner of the screen each frame.
void PrintStatusEntries()
{
	//Print Engine Options
	NCLDebug::AddStatusEntry(status_colour_header, "NCLTech Settings");
	NCLDebug::AddStatusEntry(status_colour, "     Physics Engine: %s (Press P to toggle)", PhysicsEngine::Instance()->IsPaused() ? "Paused  " : "Enabled ");
	//NCLDebug::AddStatusEntry(status_colour, "     Monitor V-Sync: %s (Press V to toggle)", GraphicsPipeline::Instance()->GetVsyncEnabled() ? "Enabled " : "Disabled");
	NCLDebug::AddStatusEntry(status_colour, "");


	//Print Performance Timers
	NCLDebug::AddStatusEntry(status_colour, "     FPS: %5.2f  (Press G for %s info)", 1000.f / timer_total.GetAvg(), show_perf_metrics ? "less" : "more");
	if (show_perf_metrics)
	{
		timer_total.PrintOutputToStatusEntry(status_colour, "          Total Time     :");
		timer_update.PrintOutputToStatusEntry(status_colour, "          Scene Update   :");
		timer_physics.PrintOutputToStatusEntry(status_colour, "          Physics Update :");
		timer_render.PrintOutputToStatusEntry(status_colour, "          Render Scene   :");

	}
	NCLDebug::AddStatusEntry(status_colour, "");
	NCLDebug::AddStatusEntry(status_colour, "Bytes: " + std::to_string(HeapFactory::Instance()->PrintDebugInfo()));
	NCLDebug::AddStatusEntry(status_colour, "Peak Bytes: " + std::to_string(HeapFactory::Instance()->PrintPeakInfo()));
	//NCLDebug::AddStatusEntry(status_colour, "Net Allocation: " + std::to_string(HeapFactory::Instance()->PrintNetAllocInfo()));

}

// Process Input
//  - Handles all program wide keyboard inputs for
//    things such toggling the physics engine and 
//    cycling through scenes.
void HandleKeyboardInputs()
{

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P));
	//PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());

	//if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_V))
	//	GraphicsPipeline::Instance()->SetVsyncEnabled(!GraphicsPipeline::Instance()->GetVsyncEnabled());

	uint sceneIdx = SceneManager::Instance()->GetCurrentSceneIndex();
	uint sceneMax = SceneManager::Instance()->SceneCount();

	/*if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Y))
	SceneManager::Instance()->JumpToScene((sceneIdx + 1) % sceneMax);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_T))
	SceneManager::Instance()->JumpToScene((sceneIdx == 0 ? sceneMax : sceneIdx) - 1);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_R))
	SceneManager::Instance()->JumpToScene(sceneIdx);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_O))
	OcTree::toggle();*/

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_G))
		show_perf_metrics = !show_perf_metrics;


	Vector3 pos;
	std::vector<Vector3> posList;

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_M))
	{

		pos = GameLogic::Instance()->getSoftPlayer(0)->getBottom()->Physics()->GetPosition();
		std::ofstream myfile;
		myfile.open("pos.txt", std::ios_base::app);
		cout << pos << "\n";

		if (myfile.is_open())
		{
			myfile << pos << "\n";
			myfile.close();
		}
	}
}


// Program Entry Point
int main()
{
	GameLogic::Instance()->setIsServer(true);
	bool show_status_menu = false;
	bool showEndScreen = false;
	//Initialize our Window, Physics, Scenes etc
	Initialize();
	//GraphicsPipeline::Instance()->SetVsyncEnabled(false);

	Window::GetWindow().GetTimer()->GetTimedMS();
	GameLogic::Instance()->setLevelIsLoaded(false);
	GameLogic::Instance()->setGameHasStarted(false);
	//Create main game-loop
	
	while (Window::GetWindow().UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		//Start Timing

		float dt = Window::GetWindow().GetTimer()->GetTimedMS() * 0.001f;	//How many milliseconds since last update?
																			//Update Performance Timers (Show results every second)
		timer_total.UpdateRealElapsedTime(dt);
		timer_physics.UpdateRealElapsedTime(dt);
		timer_update.UpdateRealElapsedTime(dt);
		timer_render.UpdateRealElapsedTime(dt);
		//std::cout << "\nFPS: " + std::to_string(1000.f / timer_total.GetAvg());
		//Print Status Entries
		//PrintStatusEntries();

		//Handle Keyboard Inputs
		HandleKeyboardInputs();


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


		if (GameLogic::Instance()->levelIsLoaded())
		{
			if (GameLogic::Instance()->gameHasStarted()) {
				GameLogic::Instance()->increaseActualGameTime(dt);
			}
			GameLogic::Instance()->increaseTotalTime(dt);
			GraphicsPipeline::Instance()->RenderScene(dt);

		}

		//Print Status Entries
		if (show_status_menu)
		{
			NCLDebug::_ClearDebugLists();
			PrintStatusEntries();

		}
		else if (showEndScreen) {
			NCLDebug::_ClearDebugLists();
			NCLDebug::AddHUD2(nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f), "Player 1 WINS!");
			NCLDebug::AddHUD(nclgl::Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f), "Press 1 to Go to the Next Level");
			NCLDebug::AddHUD(nclgl::Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f), "Press 2 to Exit");
			//PrintStatusEntries();
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1))
			{
				SceneManager::Instance()->JumpToScene("Team Project");
			}
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))
			{
				SceneManager::Instance()->JumpToScene("Main Menu");
			}
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_0)) {
			show_status_menu = !show_status_menu;
		}
		else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_5)) {
			showEndScreen = !showEndScreen;
			PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
		}





		{
			//Forces synchronisation if vsync is disabled
			// - This is solely to allow accurate estimation of render time
			// - We should NEVER normally lock our render or game loop!		
			//	glClientWaitSync(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, NULL), GL_SYNC_FLUSH_COMMANDS_BIT, 1000000);
		}
		timer_render.EndTimingSection();



		//Finish Timing
		timer_total.EndTimingSection();
		server.ServiceNetwork(dt, [&](const ENetEvent& evnt)
		{
			if (evnt.type == ENET_EVENT_TYPE_CONNECT)
			{
				printf("- New Client Connected\n");
				MySocket LobbyConnection("LBCN");
				PlayerMap.push_back(evnt.peer);
				bin_players += pow(2, PlayerMap.size() - 1);
				ReadyMap[evnt.peer] = FALSE;
				LobbyConnection.AddVar(to_string(PlayerMap.size() - 1));
				LobbyConnection.SendPacket(evnt.peer);
			}
			else if (evnt.type == ENET_EVENT_TYPE_RECEIVE) {
				MySocket Received(evnt.packet);
				string SocketId = Received.GetPacketId();
				if (SocketId == "CNCN" || SocketId == "REDY") {
					if (SocketId == "REDY") ReadyMap[evnt.peer] = !ReadyMap[evnt.peer];
					int readys = 0;
					map <ENetPeer*, bool> ::iterator itr;
					for (itr = ReadyMap.begin(); itr != ReadyMap.end(); ++itr) {
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
						if (num_p & 0b0001) GameLogic::Instance()->addSoftPlayer(0);
						if (num_p & 0b0010) GameLogic::Instance()->addSoftPlayer(1);
						if (num_p & 0b0100) GameLogic::Instance()->addSoftPlayer(2);
						if (num_p & 0b1000) GameLogic::Instance()->addSoftPlayer(3);
						//Add player to scene
						for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
							SceneManager::Instance()->GetCurrentScene()->AddSoftBody(GameLogic::Instance()->getSoftPlayer(i)->getBall());
							
							SceneManager::Instance()->GetCurrentScene()->AddGameObject(GameLogic::Instance()->getSoftPlayer(i)->getBody());
						}
					}
				}
				else if (SocketId == "INFO") {
					bucket++;
					for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
						if (PlayerMap[i] == evnt.peer) {
							
							GameLogic::Instance()->getSoftPlayer(i)->getTop()->Physics()->SetForce(nclgl::Maths::Vector3(stof(Received.TruncPacket(0)), stof(Received.TruncPacket(1)), stof(Received.TruncPacket(2))));
							GameLogic::Instance()->getSoftPlayer(i)->getBottom()->Physics()->SetForce(nclgl::Maths::Vector3(stof(Received.TruncPacket(3)), stof(Received.TruncPacket(4)), stof(Received.TruncPacket(5))));
							GameLogic::Instance()->getSoftPlayer(i)->getFront()->Physics()->SetForce(nclgl::Maths::Vector3(stof(Received.TruncPacket(6)), stof(Received.TruncPacket(7)), stof(Received.TruncPacket(8))));
							GameLogic::Instance()->getSoftPlayer(i)->getBack()->Physics()->SetForce(nclgl::Maths::Vector3(stof(Received.TruncPacket(9)), stof(Received.TruncPacket(10)), stof(Received.TruncPacket(11))));
							printf("%f\n", GameLogic::Instance()->getSoftPlayer(i)->getTop()->Physics()->GetForce().x);
							printf("%f\n", GameLogic::Instance()->getSoftPlayer(i)->getTop()->Physics()->GetForce().y);
							printf("%f\n", GameLogic::Instance()->getSoftPlayer(i)->getTop()->Physics()->GetForce().z);
						}
					}
					// Here it applies all the variables to the appropriate balls
					if (bucket == PlayerMap.size()) {
						fixTimer++;
						bucket = 0;
						//if (fixTimer < 100) {
							MySocket NextPacket("NEXT");
							//for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
								//printf("%f\n", GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetPosition().x);
								//printf("%f\n", GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetPosition().y);
								//printf("%f\n", GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetPosition().z);
								//NextPacket.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetLinearVelocity().x));
								//NextPacket.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetLinearVelocity().y));
								//NextPacket.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetLinearVelocity().z));
							//}
							NextPacket.BroadcastPacket(server.m_pNetwork);
						//}
						//else {
						//	fixTimer = 0;

						//	MySocket NextPacket("FIXX");
							//for (int i = 0; i < GameLogic::Instance()->getNumPlayers(); i++) {

								//NextPacket.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetLinearVelocity().x));
								//NextPacket.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetLinearVelocity().y));
								//NextPacket.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetLinearVelocity().z));
								//NextPacket.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetPosition().x));
								//NextPacket.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetPosition().y));
								//NextPacket.AddVar(to_string(GameLogic::Instance()->getSoftPlayer(i)->Physics()->GetPosition().z));
							//}
						//	NextPacket.BroadcastPacket(server.m_pNetwork);
						//}
					}
				}
				else if (SocketId == "JUMP") {
					for (int i = 0; i < GameLogic::Instance()->getNumSoftPlayers(); i++) {
						if (PlayerMap[i] == evnt.peer) {
							nclgl::Maths::Vector3 jump(0, 20, 0);
							for (int j = 0; j < 182; ++j) {
								GameLogic::Instance()->getSoftPlayer(i)->getBall()->softball[j]->Physics()->SetLinearVelocity(GameLogic::Instance()->getSoftPlayer(i)->getBall()->softball[j]->Physics()->GetLinearVelocity() + jump);
							}
						}
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
	}
	//HeapFactory::Instance()->PrintDebugInfo();
	//Cleanup
	Quit();
	//system("pause");
	return 0;
}
void eraseElement(ENetPeer* Item) {
	//bool isFound = false;
	//vector <ENetPeer*>::iterator it;
	//for (it = PlayerMap.begin(); it != PlayerMap.end(); ++it) {
	//	if (*it == Item) {
	//		it = PlayerMap.erase(it); // After erasing, it3 is now pointing the next location.
	//		--it; // Go to the prev location because of ++it3 in the end of for loop.
	//		isFound = true;
	//	}
	//}
}
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