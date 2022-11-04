#include <iostream>
#include "PlayerAttack.h"

PlayerAttack::PlayerAttack() :
	ActionNode(_T("PlayerAttack"))
{

}

NodeState PlayerAttack::Tick()
{
	std::cout << "Player Attack / Combo : " << GetDataFromBlackBoard<int>(_T("AttackCombo")) << std::endl;

	return NodeState::SUCCESS;
}

void PlayerAttack::Stop()
{

}
