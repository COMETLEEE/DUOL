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
		// �̺�Ʈ �׽�Ʈ���Դϴ�.
		void OnLeftFoot();

		// �̺�Ʈ �׽�Ʈ���Դϴ�.
		void OnRightFoot();

		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}