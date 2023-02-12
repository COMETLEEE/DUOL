#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Animator;
}

namespace DUOLGame
{
	class MoveController : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		MoveController(DUOLGameEngine::GameObject* owner);

		virtual ~MoveController() override;

	private:
		DUOLGameEngine::Animator* _animator;

		float _moveSpeed;

		bool _isWorldMode;

	public:
		// 이벤트 테스트용입니다.
		void OnLeftFoot();

		// 이벤트 테스트용입니다.
		void OnRightFoot();

		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}