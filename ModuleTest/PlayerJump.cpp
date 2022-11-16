#include <iostream>
#include "PlayerJump.h"

PlayerJump::PlayerJump() :
	ActionNode(_T("PlayerJump"))
{

}

NodeState PlayerJump::Tick()
{
	std::cout << "Player Jump" << std::endl;

	return NodeState::SUCCESS;
}

void PlayerJump::Stop()
{

}