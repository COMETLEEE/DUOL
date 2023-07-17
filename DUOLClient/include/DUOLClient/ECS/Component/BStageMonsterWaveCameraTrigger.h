#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class DUOL_CLIENT_API BStageMonsterWaveCameraTrigger : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		BStageMonsterWaveCameraTrigger(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("BStageMonsterWaveCameraTrigger"));

		virtual ~BStageMonsterWaveCameraTrigger() override;


	public:
		void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

			RTTR_REGISTRATION_FRIEND
	};

}