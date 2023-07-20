#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief  계단 오르고 밟는 트리거
	 */
	class DUOL_CLIENT_API CStageMiddleBossAction : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		CStageMiddleBossAction(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("CStageMiddleBossAction"));

		virtual ~CStageMiddleBossAction() override;


	public:
		void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

			RTTR_REGISTRATION_FRIEND
	};

}