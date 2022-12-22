#pragma once
#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/ActionNode.h"

namespace DUOLGame
{
	using namespace DUOLGameEngine;

	class PlayerAttack : public ActionNode
	{
	public:

		PlayerAttack();

		~PlayerAttack() final;

	public:

		NodeState Tick() final;

		void Stop() final;
	};
}