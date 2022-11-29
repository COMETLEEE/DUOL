#pragma once
#include "../DUOLGameEngine/include/DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

using namespace DUOLGameEngine;

class IsPlayerJumpable : public ConditionNode
{

public:
	IsPlayerJumpable();

	bool ConditionCheck() override;
};