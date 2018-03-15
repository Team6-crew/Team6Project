#pragma once
#include <ncltech/NetworkBase.h>
#include <nclgl\TSingleton.h>

class Network : public TSingleton<Network> {
public:
	NetworkBase * getListen() { return network; }
	void setListen(NetworkBase * listen) { network = listen; }
private:
	NetworkBase * network;
};