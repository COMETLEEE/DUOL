/**

	@file      ParticleData.h
	@brief     파티클 매니저에서 파티클을 관리하기 위한 데이터
	@details   ~ 파티클 렌더러 랩핑 + 관리 기능
	@author    SinSeongHyeon
	@date      30.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class ParticleRenderer;
}

namespace DUOLClient
{
	const DUOLCommon::tstring ParticlePath[] =
	{
	TEXT("Asset\\Particle\\test.dfx")
	};
	enum class ParticleEnum
	{
		test,
		Empty
	};

	class DUOL_CLIENT_API ParticleData : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		ParticleData(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("ParticleData"));

		virtual ~ParticleData() override;

	private:
		ParticleEnum _particleEnum;

		DUOLGameEngine::ParticleRenderer* _particleRenderer;

		bool _isOnTimer;

		float _timer;

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		void SetTimer(float time);

		void ResetData();

		bool CheckCanReturnQueue();
	public:
		friend class ParticleManager;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}
