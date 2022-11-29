#pragma once
#include "../DUOLGameEngine/include/DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

using namespace DUOLGameEngine;

class IsPlayerMoveable : public ConditionNode
{

public:
	IsPlayerMoveable();

	bool ConditionCheck() override;
};