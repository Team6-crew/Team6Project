#include "GameLogic.h"
#include "../ncltech/Player.h"
#include <ncltech\PlayerSoftBody.h>

#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>

using namespace nclgl::Maths;

GameLogic::GameLogic() {
	memset(world_paint, 0, sizeof(world_paint[0][0]) * GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE);
	cSeconds = 2;
	rad = 0.01f;
	add_rad = 0.01f;
	colours[0] = nclgl::Maths::Vector4(1.0f, 0.0f, 0.69f, 1.0f);
	colours[1] = nclgl::Maths::Vector4(0.3f, 1.0f, 1.0f, 1.0f);
	colours[2] = nclgl::Maths::Vector4(1.0f, 0.68f, 0.33f, 1.0f);
	colours[3] = nclgl::Maths::Vector4(0.0f, 1.0f, 0.02f, 1.0f);

	setnumAI(0);
	setnumOfPlayersMp(0);
	gameStarted = false;
	server = true;
	canspawn = true;
	pickupnum = 0;

	increment = 100.0f / (GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE);
}

void GameLogic::addPlayer(int num_player) {
	Player * player = new Player("Player_" + num_player,
		nclgl::Maths::Vector3(3.0f*num_player, 10.0f, 3.0f*num_player),
		1.0f,
		true,
		1.0f,
		true,
		colours[num_player]);
	player->SetPhysics(player->Physics());
	player->setControls(controls[num_player][0], controls[num_player][1], controls[num_player][2], controls[num_player][3], controls[num_player][4], controls[num_player][5]);
	player->setCamera(GraphicsPipeline::Instance()->CreateNewCamera());
	players.push_back(player);
	allPlayers.push_back(player);
	paint_perc.push_back(0.0f);
}

void GameLogic::addNetPlayer(int num_player) {
	Player * player = new Player("Player_" + num_player,
		nclgl::Maths::Vector3(3.0f*num_player, 1.f, 3.0f*num_player),
		1.0f,
		true,
		1.0f,
		true,
		colours[num_player]);
	player->SetPhysics(player->Physics());
	netPlayers.push_back(player);
	allPlayers.push_back(player);
	paint_perc.push_back(0.0f);
}


void GameLogic::addSoftPlayer(int num_splayers) {

	PlayerSoftBody* softplayer = new PlayerSoftBody("SoftPlayer_" + num_splayers,
		nclgl::Maths::Vector3(3.0f * num_splayers, 10.f, 3.0f * num_splayers),
		1.0f,
		1.0f,
		colours[num_splayers],
		num_splayers);
	for (int j = 0; j < 182; ++j)
		softplayer->getBall()->softball[j]->SetPhysics(softplayer->getBall()->softball[j]->Physics());
	softplayer->setControls(controls[num_splayers][0], controls[num_splayers][1], controls[num_splayers][2], controls[num_splayers][3], controls[num_splayers][4], controls[num_splayers][5]);
	softplayer->setCamera(GraphicsPipeline::Instance()->CreateNewCamera());
	softplayers.push_back(softplayer);
	paint_perc.push_back(0.0f);
	// setting the players name
	softplayer->setIndex(num_splayers);
}

void GameLogic::repairSoftPlayer(int num_splayers) {
	Camera* temp = getSoftPlayer(num_splayers)->getCamera();
	temp->SetYaw(0.0f);
	temp->SetPitch(-20.f);
	delete softplayers[num_splayers];
	softplayers[num_splayers] = new PlayerSoftBody("SoftPlayer_" + num_splayers,
		nclgl::Maths::Vector3(3.0f * num_splayers, 10.f, 3.0f * num_splayers),
		1.0f,
		1.0f,
		colours[num_splayers],
		num_splayers);
	for (int j = 0; j < 182; ++j)
		softplayers[num_splayers]->getBall()->softball[j]->SetPhysics(softplayers[num_splayers]->getBall()->softball[j]->Physics());
	softplayers[num_splayers]->setControls(controls[num_splayers][0], controls[num_splayers][1], controls[num_splayers][2], controls[num_splayers][3], controls[num_splayers][4], controls[num_splayers][5]);
	softplayers[num_splayers]->setCamera(temp);
}

void GameLogic::calculateProjectilePaint(float posX, float posZ, float radius, float colourZ) {
	int playerIndex = 0;
	for (int i = 0; i < softplayers.size(); i++) {
		if ((*softplayers[i]->getBottom()->Render()->GetChildIteratorStart())->GetColour().z == colourZ) {
			playerIndex = i;
		}
	}

	GameObject* ground = SceneManager::Instance()->GetCurrentScene()->FindGameObject("Ground");
	nclgl::Maths::Vector3 gr_pos = ground->Physics()->GetPosition();

	posX = (posX - gr_pos.x + WORLD_SIZE) / (WORLD_SIZE * 2);
	posZ = 1 - (posZ - gr_pos.z + WORLD_SIZE) / (WORLD_SIZE * 2);
	radius = radius / WORLD_SIZE;

	for (int i = max((posX - radius) * GROUND_TEXTURE_SIZE, 0); i < min((posX + radius) * GROUND_TEXTURE_SIZE, GROUND_TEXTURE_SIZE - 1); i++) {
		for (int j = max((posZ - radius) * GROUND_TEXTURE_SIZE, 0); j < min((posZ + radius) * GROUND_TEXTURE_SIZE, GROUND_TEXTURE_SIZE - 1); j++) {

			float in_circle = (i - posX * GROUND_TEXTURE_SIZE)*(i - posX * GROUND_TEXTURE_SIZE) + (j - posZ * GROUND_TEXTURE_SIZE)*(j - posZ * GROUND_TEXTURE_SIZE);
			if (in_circle < radius*radius * GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE) {
				if (world_paint[i][j] == 0) {
					paint_perc[playerIndex] += increment;
				}
				else if (world_paint[i][j] != playerIndex + 1) {
					paint_perc[playerIndex] += increment;
					paint_perc[world_paint[i][j] - 1] -= increment;
				}
				world_paint[i][j] = playerIndex + 1;
			}
		}
	}
}

void GameLogic::calculatePaintPercentage() {
	nclgl::Maths::Vector3 gr_pos = SceneManager::Instance()->GetCurrentScene()->FindGameObject("Ground")->Physics()->GetPosition();

	for (int k = 0; k < softplayers.size(); k++)
	{
			nclgl::Maths::Vector3 position = softplayers[k]->getBottom()->Physics()->GetPosition();
			if (position.y > 2.5f)
			{
				continue;
			}
			else
				if (softplayers[k]->getcanpaint() == false)
				{
					softplayers[k]->settime((softplayers[k]->gettime()) + 1.0f);
					if (softplayers[k]->gettime() > softplayers[k]->getDebuffTime())
					{
						softplayers[k]->setcanpaint(true);
					}
					continue;
				}
			/*else if (position.y < -3.0f)
			{
			AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"gameover.wav", false);
			continue;
			}*/
				else
				{

					add_rad = softplayers[k]->getadd_rad();
					rad = (rand() % 100) / (WORLD_SIZE*100.0f) + add_rad;

					softplayers[k]->setRadius(rad);
					posX = (position.x - gr_pos.x + WORLD_SIZE) / (WORLD_SIZE * 2);
					posZ = 1 - (position.z - gr_pos.z + WORLD_SIZE) / (WORLD_SIZE * 2);

					softplayers[k]->setRelativePosition(nclgl::Maths::Vector3(posX, position.y, posZ));

					for (int i = max((posX - rad) * GROUND_TEXTURE_SIZE, 0); i < min((posX + rad) * GROUND_TEXTURE_SIZE, GROUND_TEXTURE_SIZE - 1); i++) {
						for (int j = max((posZ - rad) * GROUND_TEXTURE_SIZE, 0); j < min((posZ + rad) * GROUND_TEXTURE_SIZE, GROUND_TEXTURE_SIZE - 1); j++) {

							float in_circle = (i - posX * GROUND_TEXTURE_SIZE)*(i - posX * GROUND_TEXTURE_SIZE) + (j - posZ * GROUND_TEXTURE_SIZE)*(j - posZ * GROUND_TEXTURE_SIZE);
							if (in_circle < rad*rad * GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE) {
								if (world_paint[i][j] == 0) {
									paint_perc[k] += increment;
								}
								else if (world_paint[i][j] != k + 1) {
									paint_perc[k] += increment;
									paint_perc[world_paint[i][j] - 1] -= increment;
								}
								world_paint[i][j] = k + 1;
							}
						}
					}
					softplayers[k]->setadd_rad(0.0f);
				}
	}

		for (int l = 0; l < aiPlayers.size(); l++)
		{
			nclgl::Maths::Vector3 position = aiPlayers[l]->Physics()->GetPosition();

			if (position.y > 2.5f)
			{
				continue;
			}
			else {
				if (aiPlayers[l]->getcanpaint() == false)
				{
					aiPlayers[l]->settime((aiPlayers[l]->gettime()) + 1.0f);
					if (aiPlayers[l]->gettime() > aiPlayers[l]->getDebuffTime())
					{
						aiPlayers[l]->setcanpaint(true);
					}
					continue;
				}
				else
				add_rad = aiPlayers[l]->getadd_rad();
				rad = (rand() % 100) / (WORLD_SIZE*100.0f) + add_rad;

				aiPlayers[l]->setRadius(rad);
				posX = (position.x - gr_pos.x + WORLD_SIZE) / (WORLD_SIZE * 2);
				posZ = 1 - (position.z - gr_pos.z + WORLD_SIZE) / (WORLD_SIZE * 2);

				aiPlayers[l]->setRelativePosition(nclgl::Maths::Vector3(posX, position.y, posZ));

				for (int i = max((posX - rad) * GROUND_TEXTURE_SIZE, 0); i < min((posX + rad) * GROUND_TEXTURE_SIZE, GROUND_TEXTURE_SIZE - 1); i++) {
					for (int j = max((posZ - rad) * GROUND_TEXTURE_SIZE, 0); j < min((posZ + rad) * GROUND_TEXTURE_SIZE, GROUND_TEXTURE_SIZE - 1); j++) {

						float in_circle = (i - posX * GROUND_TEXTURE_SIZE)*(i - posX * GROUND_TEXTURE_SIZE) + (j - posZ * GROUND_TEXTURE_SIZE)*(j - posZ * GROUND_TEXTURE_SIZE);
						if (in_circle < rad*rad * GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE) {
							if (world_paint[i][j] == 0) {
								paint_perc[l + softplayers.size()] += increment;
							}
							else if (world_paint[i][j] != l + 1) {
								paint_perc[l + softplayers.size()] += increment;
								paint_perc[world_paint[i][j] - 1] -= increment;
							}
							world_paint[i][j] = l + softplayers.size() + 1;
						}
					}
				}
				aiPlayers[l]->setadd_rad(0.0f);
			}
	}
}

void GameLogic::setControls(int x, int y, KeyboardKeys key) {
	controls[x][y] = key;
}

void GameLogic::updateControls() {
	if (numOfPlayersMp & 0b0001)
		softplayers[0]->setControls(controls[0][0], controls[0][1], controls[0][2], controls[0][3], controls[0][4], controls[0][5]);
	if (numOfPlayersMp & 0b0010)
		softplayers[1]->setControls(controls[1][0], controls[1][1], controls[1][2], controls[1][3], controls[1][4], controls[1][5]);
	if (numOfPlayersMp & 0b0100)
		softplayers[2]->setControls(controls[2][0], controls[2][1], controls[2][2], controls[2][3], controls[2][4], controls[2][5]);
	if (numOfPlayersMp & 0b1000)
		softplayers[3]->setControls(controls[3][0], controls[3][1], controls[3][2], controls[3][3], controls[3][4], controls[3][5]);
}

bool GameLogic::spawnPickup() {
	//spawn pickup
	if (GameLogic::Instance()->gameHasStarted())
	{
		int spawntime = 8;

		if (((int)totalTime % spawntime == 0) && (canspawn))
		{
			if (pickupnum < 40)
			{
				/*float pos_x = 3;
				float pos_z = -48;*/

				float pos_x = rand() % 200 - 100;
				float pos_z = rand() % 200 - 100;
				
				pickupnum = pickupnum + 1;
				lastPickupPosition = Vector3(pos_x, 20.f, pos_z);
				canspawn = false;
				return true;
			}
		}

		if (((int)totalTime % spawntime != 0) && ((int)totalTime % spawntime< spawntime))
		{
			canspawn = true;
		}
	}
	return false;
}

void GameLogic::clearGameLogic()
{
	players.clear();
	allPlayers.clear();
	netPlayers.clear();
	softplayers.clear();
	paint_perc.clear();
	aiPlayers.clear();
}