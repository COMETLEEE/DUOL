/**

	@file      ParticleRenderer.h
	@brief     Particle 을 그리기 위한 컴포넌트. (== Mesh Filter ver.Particle)
	@details   ~
	@author    ShinSeonghyeon
	@date      19.12.2022
	@copyright © Shin, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/RendererBase.h"

namespace DUOLGameEngine
{
	class Mesh;
}

namespace DUOLGameEngine
{
	class ParticleRenderer final : public DUOLGameEngine::RendererBase
	{
	public:
		ParticleRenderer(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = TEXT("ParticleRenderer"));

		virtual ~ParticleRenderer() override;
	private:
		DUOLGameEngine::Mesh* _particleMesh; // 점이긴 하지만... 메쉬는 있다.

		bool _isFirstRun; // 첫 시작인가요?

		bool _isPlay; // 재생 중인가요?

		bool _isDelayStart; // delayTime이 모두 지나서 시작 했나요?

		float _playTime; // 이 파티클의 재생시간을 기록.

		float _delayTime; // 이 파티클을 몇초뒤에 재생 시킬지.

	public:
		virtual void OnUpdate(float deltaTime) override;

		virtual void OnStart() override;

		virtual void Render() override;

		void Play();

		void Stop();
	};
}
