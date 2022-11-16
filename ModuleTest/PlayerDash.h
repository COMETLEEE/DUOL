#pragma once
#include "../DUOLGameEngine/include/DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

using namespace DUOLGameEngine;

class PlayerDash : public ActionNode
{

public:
	PlayerDash();

	NodeState Tick() override;

	void Stop() override;
};