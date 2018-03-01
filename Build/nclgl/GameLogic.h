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
	void addPlayers(int num_players);
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
	nclgl::Maths::Vector4 getColour(int pos) { return colours[pos]; };
private:
	int world_paint[GROUND_TEXTURE_SIZE][GROUND_TEXTURE_SIZE];
	std::vector<float> paint_perc;
	float posX, posZ;
	float rad;
	float add_rad;
	std::vector <Player*> players;
	std::vector <PlayerSoftBody*> softplayers;
	nclgl::Maths::Vector4 colours[4];
	float increment;
};