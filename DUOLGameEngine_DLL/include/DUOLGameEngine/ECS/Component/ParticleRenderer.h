/**

	@file      ParticleRenderer.h
	@brief     Particle 을 그리기 위한 컴포넌트. (== Mesh Filter ver.Particle)
	@details   ~
	@author    ShinSeonghyeon
	@date      19.12.2022
	@copyright © Shin, 2022. All right reserved.

**/
#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLGameEngine/ECS/Component/RendererBase.h"

namespace DUOLGameEngine
{
	class Mesh;
}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API ParticleRenderer final : public DUOLGameEngine::RendererBase
	{
	public:
		ParticleRenderer(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("ParticleRenderer"));

		virtual ~ParticleRenderer() override;
	private:
		DUOLGraphicsEngine::RenderObject _renderObjectInfo;

		DUOLGraphicsEngine::ParticleInfo _particleInfo;

		bool _isFirstRun; // 첫 시작인가요?

		bool _isPlay; // 재생 중인가요?

		bool _isDelayStart; // delayTime이 모두 지나서 시작 했나요?

		float _playTime; // 이 파티클의 재생시간을 기록.

		float _delayTime; // 이 파티클을 몇초뒤에 재생 시킬지.

		DUOLMath::Matrix _prevMatrix; // 한프레임동안 생긴 변화량에 대한 TM;

	public:
		/**
			@brief OnUpdate
		**/
		virtual void OnUpdate(float deltaTime) override;
		/**
			@brief OnStart
		**/
		virtual void OnStart() override;
		/**
			@brief 렌더링 데이터를 그래픽스 엔진으로 보냅니다.
		**/
		virtual void Render() override;
		/**
			@brief 파티클을 재생합니다.
		**/
		void Play();
		/**
			@brief 파티클을 정지합니다.
		**/
		void Stop();
		/**
			@brief ParticleData Getter
		**/
		DUOLGraphicsEngine::RenderingData_Particle& GetParticleData();
		/**
			@brief 파티클의 메쉬버퍼를 만듭니다.
		**/
		void CreateParticleBuffer();

		RTTR_ENABLE(DUOLGameEngine::RendererBase)
	};
}
