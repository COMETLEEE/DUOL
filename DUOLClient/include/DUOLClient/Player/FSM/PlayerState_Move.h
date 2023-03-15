#pragma once
#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"

namespace DUOLGameEngine
{
	class Transform;
	class Animator;
}

namespace DUOLClient
{
	class PlayerState_Move : public DUOLGameEngine::StateBase
	{
	public:
		PlayerState_Move();

		virtual ~PlayerState_Move() override;

	private:
		DUOLGameEngine::Transform* _transform;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::Transform* _cameraTransform;

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;

		friend class Player;
	};
}
