#pragma once
#include <nclgl\TSingleton.h>
#include <nclgl\Definitions.h>
#include <ncltech\SceneManager.h>
#include <ncltech\Scene.h>
#include <vector>
class GameLogic : public TSingleton<GameLogic> {

public:
	GameLogic();
	~GameLogic() {}

	void calculatePaintPercentage();
	void addPlayer(int num_players);
	float getPosX() { return posX; }
	float getPosZ() { return posZ; }
	float getPaintPerc() { return paint_perc; }
	float getRadius() { return rad;  }
	int getNumPlayers() { return players.size(); }
	Player* getPlayer(int num_player) { return players[num_player]; }

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
	float paint_perc;
	float posX, posZ;
	float rad;
	std::vector <Player*> players;
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
};