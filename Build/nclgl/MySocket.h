#pragma once
#include <enet\enet.h>
#include <iostream>
#include <sstream>
#include <vector>


using namespace std;

class MySocket {
public:
	MySocket(string data);
	MySocket(ENetPacket * packet);
	~MySocket() {}
	void SendPacket(ENetPeer * peerConnection);
	void BroadcastPacket(ENetHost * hostConnection);
	void AddVar(string cur_var);
	string GetPacketId() { return id; }
	string TruncPacket(int length);
private:
	string GetPacketId(enet_uint8 * data);
	string message;
	string id;
};
