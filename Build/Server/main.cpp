
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

//Needed to get computer adapter IPv4 addresses via windows
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")


#define SERVER_PORT 1234
#define UPDATE_TIMESTEP (1.0f / 30.0f) //send 30 position updates per second
void eraseElement(ENetPeer* Item);
NetworkBase server;
GameTimer timer;
float accum_time = 0.0f;
float rotation = 0.0f;
vector <ENetPeer *> PlayerMap;

void Win32_PrintAllAdapterIPAddresses();

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

	timer.GetTimedMS();
	while (true)
	{
		float dt = timer.GetTimedMS() * 0.001f;
		accum_time += dt;
		rotation += 0.5f * PI * dt;

		//Handle All Incoming Packets and Send any enqued packets
		server.ServiceNetwork(dt, [&](const ENetEvent& evnt)
		{
			if (evnt.type == ENET_EVENT_TYPE_CONNECT)
			{
				printf("- New Client Connected\n");
				MySocket LobbyConnection("LBCN");
				LobbyConnection.SendPacket(evnt.peer);
				PlayerMap.push_back(evnt.peer);
			}
			else if (evnt.type == ENET_EVENT_TYPE_RECEIVE){
				MySocket Received(evnt.packet);
				string SocketId = Received.GetPacketId();
				if (SocketId == "CNCN") {
					MySocket PlayersConnected("PLCN");
					PlayersConnected.AddVar(to_string(PlayerMap.size()));
					PlayersConnected.BroadcastPacket(server.m_pNetwork);
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