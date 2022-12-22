#pragma once
#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/ActionNode.h"

namespace DUOLGame
{
	using namespace DUOLGameEngine;

	class PlayerMove : public ActionNode
	{
	public:

		PlayerMove();

		~PlayerMove() final;

	public:

		NodeState Tick() final;

		void Stop() final;
	};
}