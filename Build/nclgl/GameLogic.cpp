#include "GameLogic.h"
#include "../ncltech/Player.h"
GameLogic::GameLogic() {
	memset(world_paint, 0, sizeof(world_paint[0][0]) * GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE);
	paint_perc = 0.0f;
	rad = 0.01f;
	
}

void GameLogic::addPlayers(int num_players) {
	for (int i = 0; i < num_players; i++) {
		Player * player = new Player("Player_"+i,
			Vector3(3.0f*i, 1.f, 3.0f*i),
			1.0f,
			true,
			1.0f,
			true,
			Vector4(0.2f*i, 0.2f*i, 1.0f, 1.0f));
		player->SetPhysics(player->Physics());
		switch (i) {
		case 0:
			player->setControls(KEYBOARD_I, KEYBOARD_K, KEYBOARD_J, KEYBOARD_L, KEYBOARD_SPACE);
			break;
		case 1:
			player->setControls(KEYBOARD_8, KEYBOARD_5, KEYBOARD_4, KEYBOARD_6, KEYBOARD_0);
			break;
		}
		players.push_back(player);
	}
	
}


void GameLogic::calculatePaintPercentage() {
	

	for (int i = 0; i < players.size(); i++) {
		Vector3 gr_pos = SceneManager::Instance()->GetCurrentScene()->FindGameObject("Ground")->Physics()->GetPosition();
		Vector3 position = players[i]->Physics()->GetPosition();
		rad = (rand() % 100)*0.0001f;

		players[i]->setRadius(rad);
		posX = (position.x - gr_pos.x + WORLD_SIZE) / (WORLD_SIZE * 2);
		posZ = 1 - (position.z - gr_pos.z + WORLD_SIZE) / (WORLD_SIZE * 2);

		players[i]->setRelativePosition(Vector3(posX, position.y, posZ));

		for (int i = max((posX - rad) * GROUND_TEXTURE_SIZE, 0); i < min((posX + rad) * GROUND_TEXTURE_SIZE, GROUND_TEXTURE_SIZE - 1); i++) {
			for (int j = max((posZ - rad) * GROUND_TEXTURE_SIZE, 0); j < min((posZ + rad) * GROUND_TEXTURE_SIZE, GROUND_TEXTURE_SIZE - 1); j++) {

				float in_circle = (i - posX * GROUND_TEXTURE_SIZE)*(i - posX * GROUND_TEXTURE_SIZE) + (j - posZ * GROUND_TEXTURE_SIZE)*(j - posZ * GROUND_TEXTURE_SIZE);
				if (in_circle < rad*rad * GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE) {
					if (world_paint[i][j] == 0) {
						paint_perc += 100.0f / (GROUND_TEXTURE_SIZE * GROUND_TEXTURE_SIZE);
					}
					world_paint[i][j] = 1;
				}
			}
		}
	}
}