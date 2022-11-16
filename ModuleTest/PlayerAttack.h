#pragma once
#include "../DUOLGameEngine/include/DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

using namespace DUOLGameEngine;

class PlayerAttack : public ActionNode
{

public:
	PlayerAttack();

	NodeState Tick() override;

	void Stop() override;
};