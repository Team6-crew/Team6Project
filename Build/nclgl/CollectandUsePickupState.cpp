#include "CollectandUsePickupState.h"
#include <nclgl\AI\ChasingState.h>
#include <nclgl\AI\StateMachine.h>

class SpeedPickup;
class CollectAndUsePickupState;


using namespace nclgl::Maths;

void CollectandUsePickupState::enter(StateMachine * sOwner)
{
	cout << "Entered Pickup State \n";
}

void CollectandUsePickupState::update(StateMachine * sOwner)
{
	/*nclgl::Maths::Vector3 ballPos = sOwner->getOwner()->Physics()->GetPosition();
	Vector3 pickupPos = Paintbomb::getPickup(1)->getLocation();
	nclgl::Maths::Vector3 steering = ((pickupPos - ballPos).Normalise()) * maxVel;

	steering = steering / sOwner->getOwner()->Physics()->GetInverseMass();

	float distanceToPickup = (pickupPos - ballPos).Length();

	if (distanceToPickup < 2)
	{
		cout << "pickup collected \n";
		sOwner->setCurrentState(sOwner, ChasingState::GetInstance());
	}

	
	sOwner->getOwner()->Physics()->SetForce(nclgl::Maths::Vector3(steering));*/
}

void CollectandUsePickupState::exit(StateMachine * sOwner)
{
	cout << "Leaving Collect and use Pickup State \n";
}
