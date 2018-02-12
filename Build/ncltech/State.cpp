#include "State.h"

void State::updateState(GameObject* Owner)
{
	
	
		Vector3 playerPos = Player::getBall()->Physics()->GetPosition();
		Vector3 AIPos = BallAI::getBall()->Physics()->GetPosition();
		float distance = (playerPos - AIPos).Length();

		if (distance < 30)
		{
			s->setCurrentState(chasingState);
		}
		if (distance >= 31)
		{
			s->setCurrentState(roamingState);
		}

}
