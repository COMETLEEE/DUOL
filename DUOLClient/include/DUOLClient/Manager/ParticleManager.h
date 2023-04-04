/**

	@file      ParticleManager.h
	@brief     파티클 매니저, 오브젝트 풀링을 통해 파티클을 관리한다.
	@details   ~ 모든 파티클들을 가지고 다른 오브젝트에 대여해주는 것.
	@author    SinSeongHyeon
	@date      30.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "DUOLClient/Export_Client.h"
#include "DUOLClient/ECS/Component/ParticleData.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class ParticleRenderer;
}

namespace DUOLClient
{
	class DUOL_CLIENT_API ParticleManager : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		ParticleManager(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("ParticleManager"));

		virtual ~ParticleManager() override;

	private:
		static ParticleManager* _instance; // Singleton을 직접 만드는 이유는 컴포넌트이기 때문에 생성자를 커스텀하기 위함.

	private:
		/**
		* \brief [파티클 Enum, 파티클 큐] 페어로 관리한다.
		*/
		std::unordered_map<ParticleEnum, std::queue<ParticleData*>> _particleQueueMap;

		/**
		* \brief 큐에 들어있는 파티클을 인스펙터에서 확인하기 위한 게임 오브젝트.
		*/
		DUOLGameEngine::GameObject* _particleQueueGameObject;

		bool _isStart;

		static int g_particleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::Empty)];
	private:
		/**
		* \brief  파티클의 게임 오브젝트가 Enable false가 되거나 파티클 렌더러가 종료 혹은 타이머가 모두 지났을 때 반환.
		*/
		DUOLGameEngine::CoroutineHandler ReturnParticle(ParticleData* particleData);
		/**
		* \brief  큐에 미리 사용할 파티클 데이터를 적재한다.
		*/
		void Initialize();
	public:
		static ParticleManager* GetInstance();

		void PushBack(ParticleData* particleData);

		DUOLGameEngine::ParticleRenderer* Pop(ParticleEnum particleEnum, float timer = std::numeric_limits<float>::max());
	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}
