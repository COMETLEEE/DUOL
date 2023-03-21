#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief 물리 현상 시연용 모델 슈터
	 */
	class DUOL_CLIENT_API TestParticle final : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		TestParticle(DUOLGameEngine::GameObject * owner = nullptr, const DUOLCommon::tstring & name = _T("TestParticle"));

		virtual ~TestParticle() override;

	public:
		bool _playOnce;

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}
