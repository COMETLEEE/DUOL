
#pragma once
#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/DecoratorNode.h"

namespace DUOLGame
{
	using namespace DUOLGameEngine;

	class PlayerGroundCondition : public DecoratorNode
	{
	public:

		PlayerGroundCondition();

		~PlayerGroundCondition() final;

	public:

		NodeState Tick() final;

		void Stop() final;
	};
}