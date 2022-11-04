#pragma once
#include "../DUOLGameEngine/include/DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

using namespace DUOLGameEngine;

class PlayerJump : public ActionNode
{

public:
	PlayerJump();

	NodeState Tick() override;

	void Stop() override;
};