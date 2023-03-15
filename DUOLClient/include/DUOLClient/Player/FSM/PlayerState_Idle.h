#pragma once
#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"

namespace DUOLGameEngine
{
	class Transform;
	class Animator;
}

namespace DUOLClient
{
	class PlayerState_Idle : public DUOLGameEngine::StateBase
	{
	public:
		PlayerState_Idle();

		virtual ~PlayerState_Idle() override;

	private:
		DUOLGameEngine::Transform* _transform;

		DUOLGameEngine::Animator* _animator;

		DUOLGameEngine::Transform* _cameraTransform;

		void LookDirectionUpdate();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;

		friend class Player;
	};
}
