#pragma once
#include <iostream>

#include "../DUOLGameEngine/include/DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"

#pragma comment(lib, "../Build/x64/Debug/DUOLGameEngine.lib")

using namespace DUOLGameEngine;

class PlayerBehaviorTree
{
	std::shared_ptr<BehaviorTree> bt;
	std::shared_ptr<BlackBoard> bb;

public:
	void Init();
	void Update(float tick);
};