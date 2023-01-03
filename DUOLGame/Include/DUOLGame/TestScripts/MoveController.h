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
		MoveController(const std::weak_ptr<DUOLGameEngine::GameObject>& owner);

		virtual ~MoveController() override;

	private:
		DUOLGameEngine::Animator* _animator;

		float _moveSpeed;

		bool _isWorldMode;

	public:
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}