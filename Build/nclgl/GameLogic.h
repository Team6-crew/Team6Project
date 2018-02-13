#pragma once
#include <nclgl\TSingleton.h>
#include <nclgl\Definitions.h>
#include <ncltech\SceneManager.h>
#include <ncltech\Scene.h>

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

private:
	int world_paint[GROUND_TEXTURE_SIZE][GROUND_TEXTURE_SIZE];
	float paint_perc;
	float posX, posZ;
	float rad;
	vector <Player*> players;
};