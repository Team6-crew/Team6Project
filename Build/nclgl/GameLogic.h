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
	void addPlayers(int num_players);
	float getPosX() { return posX; }
	float getPosZ() { return posZ; }
	float getPaintPerc() { return paint_perc; }
	float getRadius() { return rad;  }
	int getNumPlayers() { return players.size(); }
	Player* getPlayer(int num_player) { return players[num_player]; }

	// split screen ui integration
	void setnumOfPlayersMp(int nMp) { numOfPlayersMp = nMp; };
	void setnumAI(int nAI) { numAI = nAI; };

	int getnumOfPlayersMp(void) { return numOfPlayersMp; };
	int getnumAI(void) { return numAI; };

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

};