#pragma once
#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/ActionNode.h"

namespace DUOLGame
{
	using namespace DUOLGameEngine;

	class PlayerJump : public ActionNode
	{
	public:

		PlayerJump();

		~PlayerJump() final;

	public:

		NodeState Tick() final;

		void Stop() final;
	};
}