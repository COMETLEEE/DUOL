#pragma once
#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/DecoratorNode.h"

namespace DUOLGame
{
	using namespace DUOLGameEngine;

	class PlayerControlCondition : public DecoratorNode
	{
	public:

		PlayerControlCondition();

		~PlayerControlCondition() final;

	public:

		NodeState Tick() final;

		void Stop() final;
	};
}