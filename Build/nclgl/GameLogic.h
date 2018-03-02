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
	void addSoftPlayers(int num_players);
	float getPosX() { return posX; }
	float getPosZ() { return posZ; }
	std::vector<float> * getPaintPerc() { return &paint_perc; }
	float getRadius() { return rad;  }
	int getNumPlayers() { return players.size(); }
	int getNumSoftPlayers() { return softplayers.size(); }
	int getTotalPlayers() { return players.size() + softplayers.size(); }
	Player* getPlayer(int num_player) { return players[num_player]; }
	PlayerSoftBody* getSoftPlayer(int num_splayer) { return softplayers[num_splayer]; }
	void clearPlayers() { players.clear();	softplayers.clear(); }

	// split screen ui integration
	void setnumOfPlayersMp(int nMp) { numOfPlayersMp = nMp; };
	void setnumAI(int nAI) { numAI = nAI; };
	void updateControls();
	int getnumOfPlayersMp(void) { return numOfPlayersMp; };
	int getnumAI(void) { return numAI; };
	void setControls(int x, int y, KeyboardKeys key);
	KeyboardKeys getControls(int x, int y) { return controls[x][y]; }
private:
	int world_paint[GROUND_TEXTURE_SIZE][GROUND_TEXTURE_SIZE];
	std::vector<float> paint_perc;
	float posX, posZ;
	float rad;
	float add_rad;
	std::vector <Player*> players;
	std::vector <PlayerSoftBody*> softplayers;
	nclgl::Maths::Vector4 colours[4];

	// split screen ui integration
	int numOfPlayersMp;
	int numAI;
	KeyboardKeys controls[4][6] = {
	{ KEYBOARD_W, KEYBOARD_S, KEYBOARD_A, KEYBOARD_D, KEYBOARD_SPACE, KEYBOARD_Q },
	{ KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L, KEYBOARD_N, KEYBOARD_O },
	{ KEYBOARD_UP, KEYBOARD_DOWN, KEYBOARD_LEFT, KEYBOARD_RIGHT, KEYBOARD_DELETE, KEYBOARD_END },
	{ KEYBOARD_NUMPAD8, KEYBOARD_NUMPAD5, KEYBOARD_NUMPAD4, KEYBOARD_NUMPAD6, KEYBOARD_NUMPAD0, KEYBOARD_NUMPAD9 }
	};
	float increment;
};