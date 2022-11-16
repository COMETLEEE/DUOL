#pragma once
#include "../DUOLGameEngine/include/DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

using namespace DUOLGameEngine;

class IsPlayerAttackable : public ConditionNode
{

public:
	IsPlayerAttackable();

	bool ConditionCheck() override;
};