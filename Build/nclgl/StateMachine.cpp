#pragma once
#include <nclgl\AI\StateMachine.h>

DEFINE_HEAP(StateMachine, "AI");

void StateMachine::setCurrentState(StateMachine* sOwner, State* newState)
{
		if (newState != nullptr)
		{
			if (currentState != nullptr)
			{
				currentState->exit(sOwner);
			}
			previousState = currentState;
			sOwner->setPreviousState(previousState);
			currentState = newState;
			newState->enter(sOwner);

		}
	
}
