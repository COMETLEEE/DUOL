#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class EnemyGroupController;

	class DUOL_CLIENT_API BossConditionalWaveHandler : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		BossConditionalWaveHandler(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("BossConditionalWaveHandler"));

		virtual ~BossConditionalWaveHandler() override;

	private:
		void OnAwake() override;

		void OnUpdate(float deltaTime) override;

		bool _isCleared;

		DUOLCommon::tstring _bossTrigger;

		EnemyGroupController* _bossEnemyTrigger;

		DUOLCommon::tstring _waveTrigger;

		EnemyGroupController* _waveEnemyTrigger;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
		RTTR_REGISTRATION_FRIEND
	};
}