#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/FiniteStateMachine/FiniteStateMachine.h"

namespace DUOLClient
{
	/**
	 * \brief Player State Machine
	 */
	class Player : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		Player(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Player"));

		virtual ~Player();

	private:
		DUOLGameEngine::FiniteStateMachine _playerStateMachine;
		
		DUOLGameEngine::Transform* _cameraTransform;

	private:
		void InitializeStateMachine();

	public:
		virtual void OnStart() override;
		
		virtual void OnLateUpdate(float deltaTime) override;
	};
}
