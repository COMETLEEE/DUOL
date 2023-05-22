#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"


namespace DUOLClient
{
	/**
	 * \brief 물리 현상 시연용 모델 슈터
	 */
	class DUOL_CLIENT_API MaterialController final : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		MaterialController();
		MaterialController(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = _T("MaterialController"));

		virtual ~MaterialController() override;

	public:
		DUOLGameEngine::GameObject* meshobj;

		bool checkOnce;

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}
