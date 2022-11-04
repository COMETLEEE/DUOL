#pragma once
#include "../DUOLGameEngine/include/DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

using namespace DUOLGameEngine;

class PlayerIdle : public ActionNode
{

public:
	PlayerIdle();

	NodeState Tick() override;

	void Stop() override;
};