#pragma once
#include <nclgl\TSingleton.h>
#include <nclgl\Definitions.h>
#include <ncltech\SceneManager.h>
#include <ncltech\Scene.h>
#include <vector>
#include <ncltech\PlayerSoftBody.h>

class GameLogic : public TSingleton<GameLogic> {

public:
	GameLogic();
	~GameLogic() {}

	void calculatePaintPercentage();
	void addPlayer(int num_players);
	void addNetPlayer(int num_players);
	void addSoftPlayers(int num_players);
	float getPosX() { return posX; }
	float getPosZ() { return posZ; }
	std::vector<float> * getPaintPerc() { return &paint_perc; }
	float getRadius() { return rad;  }
	int getNumPlayers() { return players.size(); }
	int getNumNetPlayers() { return netPlayers.size(); }
	int getNumSoftPlayers() { return softplayers.size(); }
	int getTotalPlayers() { return players.size() + softplayers.size(); }

	int getNumAllPlayers() { return allPlayers.size(); }
	Player* getAllPlayer(int num_player) { return allPlayers[num_player]; }
	Player* getNetPlayer(int num_player) { return netPlayers[num_player]; }

	Player* getPlayer(int num_player) { return players[num_player]; }
	PlayerSoftBody* getSoftPlayer(int num_splayer) { return softplayers[num_splayer]; }
	void clearPlayers() { players.clear();	softplayers.clear(); }

	// split screen ui integration
	void setnumOfPlayersMp(int nMp) { numOfPlayersMp = nMp; };
	void setnumOfNetPlayers(int nMp) { numOfNetPlayers = nMp; };
	void setnumAI(int nAI) { numAI = nAI; };
	void updateControls();
	int getnumOfPlayersMp(void) { return numOfPlayersMp; };
	int getnumOfNetPlayers(void) { return numOfNetPlayers; };
	int getnumAI(void) { return numAI; };
	void setControls(int x, int y, KeyboardKeys key);
	KeyboardKeys getControls(int x, int y) { return controls[x][y]; }

	void setMyNetNum(int k) { myNetPlayerNum = k; }
	int getMyNetNum() { return myNetPlayerNum; }
private:
	int myNetPlayerNum = 0;
	int world_paint[GROUND_TEXTURE_SIZE][GROUND_TEXTURE_SIZE];
	std::vector<float> paint_perc;
	float posX, posZ;
	float rad;
	float add_rad;
	std::vector <Player*> players;
	std::vector <Player*> allPlayers;
	std::vector <Player*> netPlayers;
	std::vector <PlayerSoftBody*> softplayers;
	nclgl::Maths::Vector4 colours[4];
	// split screen ui integration
	int numOfPlayersMp;
	int numOfNetPlayers;
	int numAI;
	KeyboardKeys controls[4][6] = {
	{ KEYBOARD_W, KEYBOARD_S, KEYBOARD_A, KEYBOARD_D, KEYBOARD_SPACE, KEYBOARD_Q },
	{ KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L, KEYBOARD_N, KEYBOARD_O },
	{ KEYBOARD_UP, KEYBOARD_DOWN, KEYBOARD_LEFT, KEYBOARD_RIGHT, KEYBOARD_DELETE, KEYBOARD_END },
	{ KEYBOARD_NUMPAD8, KEYBOARD_NUMPAD5, KEYBOARD_NUMPAD4, KEYBOARD_NUMPAD6, KEYBOARD_NUMPAD0, KEYBOARD_NUMPAD9 }
	};
	float increment;
};