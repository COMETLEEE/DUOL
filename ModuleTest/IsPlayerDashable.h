#pragma once
#include "../DUOLGameEngine/include/DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

using namespace DUOLGameEngine;

class IsPlayerDashable : public ConditionNode
{

public:
	IsPlayerDashable();

	bool ConditionCheck() override;
};