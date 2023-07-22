#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class EnemyGroupController;

	class DUOL_CLIENT_API BossZoneTrigger : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		BossZoneTrigger(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("BossZoneTrigger"));

		virtual ~BossZoneTrigger() override;


	public:
		void OnAwake() override;

		void  OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		void OnUpdate(float deltaTime) override;

		DUOLGameEngine::GameObject* _cautionCollider;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

			RTTR_REGISTRATION_FRIEND
	};

}