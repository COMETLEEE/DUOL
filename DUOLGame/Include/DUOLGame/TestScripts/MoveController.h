#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGame
{
	class MoveController : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		MoveController(const std::weak_ptr<DUOLGameEngine::GameObject>& owner);

		virtual ~MoveController() override;

	private:
		float _moveSpeed;

		bool _isWorldMode;

	public:
		virtual void OnUpdate(float deltaTime) override;
	};
}