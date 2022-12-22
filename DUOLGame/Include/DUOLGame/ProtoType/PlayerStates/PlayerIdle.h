#pragma once
#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/ActionNode.h"

namespace DUOLGame
{
	using namespace DUOLGameEngine;

	class PlayerIdle : public ActionNode
	{
	public:

		PlayerIdle();

		~PlayerIdle() final;

	public:

		NodeState Tick() final;

		void Stop() final;
	};
}