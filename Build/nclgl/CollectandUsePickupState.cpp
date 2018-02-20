#include "CollectandUsePickupState.h"
#include <ncltech\SpeedPickup.h>
#include <nclgl\AI\ChasingState.h>
#include <nclgl\AI\StateMachine.h>

class SpeedPickup;
class CollectAndUsePickupState;


using namespace nclgl::Maths;

void CollectandUsePickupState::enter(StateMachine * sOwner, GameObject * owner)
{
	cout << "Entered Pickup State \n";
}

void CollectandUsePickupState::update(StateMachine * sOwner, GameObject * owner)
{
	nclgl::Maths::Vector3 ballPos = owner->Physics()->GetPosition();
	Vector3 pickupPos = SpeedPickup::getPickup(1)->getLocation();
	nclgl::Maths::Vector3 steering = ((pickupPos - ballPos).Normalise()) * maxVel;

	steering = steering / owner->Physics()->GetInverseMass();

	float distanceToPickup = (pickupPos - ballPos).Length();

	if (distanceToPickup < 2)
	{
		cout << "pickup collected \n";
		sOwner->setCurrentState(sOwner, ChasingState::GetInstance());
	}

	
	owner->Physics()->SetForce(nclgl::Maths::Vector3(steering));
}

void CollectandUsePickupState::exit(StateMachine * sOwner, GameObject * owner)
{
	cout << "Leaving Collect and use Pickup State \n";
}
