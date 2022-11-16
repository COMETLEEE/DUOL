#pragma once
#include "../DUOLGameEngine/include/DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

using namespace DUOLGameEngine;

class PlayerMove : public ActionNode
{

public:
	PlayerMove();

	NodeState Tick() override;

	void Stop() override;
};