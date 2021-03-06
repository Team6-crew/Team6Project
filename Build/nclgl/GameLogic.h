#pragma once
#include <nclgl\TSingleton.h>
#include <nclgl\Definitions.h>
#include <ncltech\SceneManager.h>
#include <ncltech\Scene.h>
#include <vector>
#include <ncltech\PlayerSoftBody.h>
#include <map>

class GameLogic : public TSingleton<GameLogic> {

public:
	GameLogic();
	~GameLogic() {}

	void calculatePaintPercentage();
	void addPlayer(int num_players);
	void addNetPlayer(int num_players);
	void addSoftPlayer(int num_players);
	float getPosX() { return posX; }
	float getPosZ() { return posZ; }
	std::vector<float> * getPaintPerc() { return &paint_perc; }
	float getRadius() { return rad;  }
	int getNumPlayers() { return (int)players.size(); }
	int getNumNetPlayers() { return (int)netPlayers.size(); }
	int getNumSoftPlayers() { return (int)softplayers.size(); }
	int getNumAIPlayers() { return (int)aiPlayers.size(); }
	int getTotalPlayers() { return (int)(players.size() + softplayers.size());}
	float getTotalTime() { return totalTime; }
	bool levelIsLoaded() { return levelLoaded; }
	bool gameHasStarted() { return gameStarted; }
	void increaseTotalTime(float dt) { totalTime += dt; }
	void setTotalTime(float t) { totalTime = t; }
	void setLevelIsLoaded(bool l) { levelLoaded = l; }
	void setGameHasStarted(bool l) { gameStarted = l; }
	
	float getActualGameTime() { return actualGameTime; }
	void setActualGameTime(float t) { actualGameTime = t; }
	void increaseActualGameTime(float dt) { actualGameTime +=dt; }
	
	bool isServer() { return server; }
	void setIsServer(bool s) { server = s; }

	bool spawnPickup();
	nclgl::Maths::Vector3 getLastPickupPosition() { return lastPickupPosition; }
	int getNumAllPlayers() { return (int)allPlayers.size(); }
	Player* getAllPlayer(int num_player) { return allPlayers[num_player]; }
	PlayerSoftBody* getNetPlayer(int num_player) { return netPlayers[num_player]; }
	BallAI* getAIPlayer(int num_player) { return aiPlayers[num_player]; }

	Player* getPlayer(int num_player) { return players[num_player]; }
	PlayerSoftBody* getSoftPlayer(int num_splayer) { return softplayers[num_splayer]; }
	
	void clearPlayers() { players.clear();	softplayers.clear(); }
	void calculateProjectilePaint(float posX, float posZ, float radius, float colourZ );
	
	
	void addAIPlayer(BallAI* a) { aiPlayers.push_back(a); paint_perc.push_back(0.0f);};

	void repairSoftPlayer(int num_splayers);
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

	void clearGameLogic();

	void SetPlayerCapturedObject(GameObject * go, int p) { PlayerCapturedObject[go] = p; }
	int  GetPlayerCapturedObject(GameObject * go) { return PlayerCapturedObject.at(go); }

	void setPaintPerc(int p, float sc) { paint_perc[p] += sc; }
	float getPaintPerc(int p) { return paint_perc[p]; }

	int getSeconds() { return seconds; }
	void setSeconds(int sec) { seconds = sec; }
	nclgl::Maths::Vector4 getColours(int col) { return colours[col]; }

	void setCurrentTime(int cSec) { cSeconds = cSec; };
	int getCurrentTime() { return cSeconds; }

	bool getIsGamePaused() { return isGamePaused; }
	void setIsGamePaused(bool gamePaused) { isGamePaused = gamePaused; }

	void setMyNetNum(int k) { myNetPlayerNum = k; }
	int getMyNetNum() { return myNetPlayerNum; }

	bool getJustJumped() { return justJumped; }
	void setJustJumped(bool k) { justJumped = k; }

private:
	bool justJumped = false;
	int myNetPlayerNum = 0;
	int world_paint[GROUND_TEXTURE_SIZE][GROUND_TEXTURE_SIZE];
	std::vector<float> paint_perc;
	float posX, posZ;
	float rad;
	float add_rad;
	std::vector <Player*> players;
	std::vector <Player*> allPlayers;
	std::vector <PlayerSoftBody*> netPlayers;
	std::vector <PlayerSoftBody*> softplayers;
	std::vector <BallAI*> aiPlayers;
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
	bool levelLoaded;
	float totalTime;
	bool gameStarted;
	float actualGameTime;

	bool server;

	nclgl::Maths::Vector3 lastPickupPosition;
	bool canspawn;
	int pickupnum;
	map <GameObject * , int> PlayerCapturedObject;
	int seconds;
	int cSeconds;
	bool isGamePaused;

	float totalscore;
};