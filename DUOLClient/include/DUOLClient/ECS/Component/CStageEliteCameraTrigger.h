#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class DUOL_CLIENT_API CStageEliteCameraTrigger : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		CStageEliteCameraTrigger(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("CStageEliteCameraTrigger"));

		virtual ~CStageEliteCameraTrigger() override;


	public:
		void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

			RTTR_REGISTRATION_FRIEND
	};

}