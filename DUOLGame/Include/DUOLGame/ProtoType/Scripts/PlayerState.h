
#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/StateMachine/BehaviorTree/BehaviorTree.h"
#include <DUOLCommon/ImGui/imgui.h>

namespace DUOLGame
{
	using namespace DUOLGameEngine;

	class PlayerState : public MonoBehaviourBase
	{
	public:

		PlayerState(const std::weak_ptr<GameObject>& owner);

		~PlayerState() final;

	private:
		std::shared_ptr<BehaviorTree> _bt;

		std::shared_ptr<BlackBoard> _bb;

	public:
		float _moveSpeed;

		float _jumpPower;

		bool _isGround;

	public:

		void OnAwake() final;

		void OnStart() final;

		void OnUpdate(float deltaTime) final;

		void SetActive(bool value);
	};
}