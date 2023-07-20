#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief  계단 오르고 밟는 트리거
	 */
	class DUOL_CLIENT_API AStageAreaCameraTirgger : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		AStageAreaCameraTirgger(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("AStageAreaCameraTirgger"));

		virtual ~AStageAreaCameraTirgger() override;


	public:
		void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

			RTTR_REGISTRATION_FRIEND
	};

}