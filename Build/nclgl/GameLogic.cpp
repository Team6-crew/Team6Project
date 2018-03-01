#include "GameLogic.h"
#include "../ncltech/Player.h"
#include <ncltech\PlayerSoftBody.h>

GameLogic::GameLogic() {
	memset(world_paint, 0, sizeof(world_paint[0][0]) * GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE);
	
	rad = 0.01f;
	add_rad = 0.01f;
	colours[0] = nclgl::Maths::Vector4(1.0f, 0.0f, 0.69f, 1.0f);
	colours[1] = nclgl::Maths::Vector4(0.3f, 1.0f, 1.0f, 1.0f);
	colours[2] = nclgl::Maths::Vector4(1.0f, 0.68f, 0.33f, 1.0f);
	colours[3] = nclgl::Maths::Vector4(0.0f, 1.0f, 0.02f, 1.0f);
	increment = 100.0f / (GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE);
}

void GameLogic::addPlayers(int num_players) {
	for (int i = 0; i < num_players; i++) {
		Player * player = new Player("Player_"+i,
			nclgl::Maths::Vector3(3.0f*i + 6, 1.f, 3.0f*i + 6),
			1.0f,
			true,
			1.0f,
			true,
			colours[i]);
		player->SetPhysics(player->Physics());
		switch (i) {
		case 0:
			player->setControls(KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L, KEYBOARD_SPACE, KEYBOARD_N);
			break;
		case 1:
			player->setControls(KEYBOARD_NUMPAD8, KEYBOARD_NUMPAD5, KEYBOARD_NUMPAD4, KEYBOARD_NUMPAD6, KEYBOARD_NUMPAD0, KEYBOARD_NUMPAD9);
			break;
		}
		player->setCamera(GraphicsPipeline::Instance()->CreateNewCamera());
		players.push_back(player);
		paint_perc.push_back(0.0f);
	}
	
}

void GameLogic::addSoftPlayers(int num_splayers) {
	for (int i = 0; i < num_splayers; i++) {
		PlayerSoftBody* softplayer = new PlayerSoftBody("SoftPlayer_" + i,
			nclgl::Maths::Vector3(3.0f*i, 1.f, 3.0f*i),
			1.0f,
			1.0f,
			colours[i]);
		for (int j = 0; j < 182; ++j)
		softplayer->getBall()->softball[j]->SetPhysics(softplayer->getBall()->softball[j]->Physics());
		switch (i) {
		case 0:
			softplayer->setControls(KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L, KEYBOARD_SPACE, KEYBOARD_N);
			break;
		case 1:
			softplayer->setControls(KEYBOARD_NUMPAD8, KEYBOARD_NUMPAD5, KEYBOARD_NUMPAD4, KEYBOARD_NUMPAD6, KEYBOARD_NUMPAD0, KEYBOARD_NUMPAD9);
			break;
		}
		softplayer->setCamera(GraphicsPipeline::Instance()->CreateNewCamera());
		softplayers.push_back(softplayer);
		paint_perc.push_back(0.0f);
	}
}
void GameLogic::calculatePaintPercentage() {
	GameObject* ground = SceneManager::Instance()->GetCurrentScene()->FindGameObject("Ground");
	nclgl::Maths::Vector3 gr_pos = ground->Physics()->GetPosition();

	
	for (int k = 0; k < players.size(); k++) {
		nclgl::Maths::Vector3 position = players[k]->Physics()->GetPosition();
		if (position.y > gr_pos.y + 1.0f + 1.0f) //ground half dims and player rad
		{
			continue;
		}
		else
		{
			add_rad = players[k]->getadd_rad();
			if (add_rad == 0)
			{
				rad = (rand() % 100)*0.0001f;
			}
			else
			{
				rad = (rand() % 100)*0.0001f + add_rad;
			}
			players[k]->setRadius(rad);
			posX = (position.x - gr_pos.x + WORLD_SIZE) / (WORLD_SIZE * 2);
			posZ = 1 - (position.z - gr_pos.z + WORLD_SIZE) / (WORLD_SIZE * 2);

			players[k]->setRelativePosition(nclgl::Maths::Vector3(posX, position.y, posZ));

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
			players[k]->setadd_rad(0.0f);
		}
	}
}