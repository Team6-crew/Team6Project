#pragma once
#include <nclgl\TSingleton.h>
#include <nclgl\Definitions.h>
#include <ncltech\SceneManager.h>
#include <ncltech\Scene.h>
#include <vector>
class AIBall;
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
	int getNumAIPlayers() { return aiPlayers.size(); }
	Player* getPlayer(int num_player)  {return players[num_player]; }
	BallAI* getAIPlayer(int num_player) { return aiPlayers[num_player]; }
	void addAIPlayer(BallAI* a) { aiPlayers.push_back(a); };

private:
	int world_paint[GROUND_TEXTURE_SIZE][GROUND_TEXTURE_SIZE];
	float paint_perc;
	float posX, posZ;
	float rad;
	std::vector <Player*> players;
	std::vector <BallAI*> aiPlayers;
	nclgl::Maths::Vector4 colours[4];
};