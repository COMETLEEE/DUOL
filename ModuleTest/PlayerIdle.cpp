#include <iostream>
#include "PlayerIdle.h"

PlayerIdle::PlayerIdle() :
	ActionNode(_T("PlayerIdle"))
{

}

NodeState PlayerIdle::Tick()
{
	std::cout << "Player Idle" << std::endl;

	return NodeState::SUCCESS;
}

void PlayerIdle::Stop()
{

}