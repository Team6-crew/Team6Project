#include "MySocket.h"

MySocket::MySocket(ENetPacket * packet) {
	id = GetPacketId(packet->data);
	stringstream ss;
	for (int i = 4; i < packet->dataLength - 1; i++) {
		ss << packet->data[i];
	}
	message = ss.str();
}

MySocket::MySocket(string data) {
	id = data;
	message = "";
}

string MySocket::GetPacketId(enet_uint8 * data) {
	std::stringstream ss;
	ss << data[0] << data[1] << data[2] << data[3];
	return ss.str();
}

void MySocket::BroadcastPacket(ENetHost * hostConnection) {
	string sock = id + message;
	ENetPacket* packet = enet_packet_create(sock.c_str(), strlen(sock.c_str()) + 1, 0);
	enet_host_broadcast(hostConnection, 0, packet);
}

void MySocket::SendPacket(ENetPeer * peerConnection) {
	string sock = id + message;
	ENetPacket* packet = enet_packet_create(sock.c_str(), strlen(sock.c_str()) + 1, 0);
	enet_peer_send(peerConnection, 0, packet);
}

string MySocket::TruncPacket(string data, int num) {
	stringstream ss;
	int p = -1, i = 0;

	while (i < data.length() && p<=num) {
		if (p == num && data[i] != '/') {
			ss << data[i];
		}
		if (data[i] == '/') {
			p++;
		}
		i++;
	}
	string body = ss.str();
	return body;
}

void MySocket::AddVar(string cur_var) {
	message += "/" + cur_var;
}
