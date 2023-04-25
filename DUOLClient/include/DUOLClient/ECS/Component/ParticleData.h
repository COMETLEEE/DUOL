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
		TEXT("Asset\\Particle\\test.dfx"),
		TEXT("Asset\\Particle\\MonsterHit.dfx"),
		TEXT("Asset\\Particle\\SpeedEffectBase2.dfx"),
		TEXT("Asset\\Particle\\LightningFistTestBlue.dfx"),
		TEXT("Asset\\Particle\\BuffDXZ2.dfx"),
		TEXT("Asset\\Particle\\Crack3.dfx"),
		TEXT("Asset\\Particle\\DustEffect_NoneLoopWhite.dfx"),
		TEXT("Asset\\Particle\\PunchWide4.dfx"),
		TEXT("Asset\\Particle\\Summon3.dfx")
	};

	enum class ParticleEnum
	{
		test,				// 테스트용
		MonsterHit,			// 맞을 떄 활성화
		RunShift,			// 질주 상태 (Run) 활성화
		FistForm,			// 피스트 공격 중
		OverdriveEnter,		// 오버 드라이브 상태 돌입
		Crack,				// 대지 분쇄 (SSSF, SSFF 마지막)
		Dust,				// 흙먼지 (SSSS, FFSS, FSS 마지막)
		FistWide,			// 정권 (SFF, FFF 마지막)
		MonsterSummon,		// 몬스터 소환
		Empty				// 숫자를 세기 위함
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
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;

		void SetTimer(float time);

		void ResetData();

		bool CheckCanReturnQueue() const;

	public:
		friend class ParticleManager;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};
}
