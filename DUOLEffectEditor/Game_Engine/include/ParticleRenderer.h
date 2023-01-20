#pragma once
#include "IComponents.h"
#include "Export\RenderingData.h"

namespace Muscle
{
	class ParticleRenderer : public IComponents
	{
	public:
		ParticleRenderer(std::shared_ptr<GameObject> _GameObject);

		~ParticleRenderer();

	private:
		std::shared_ptr<Muscle::Transform> _transform;

		std::shared_ptr<MuscleGrapics::RenderingData_Particle> _particleData;

		bool _isFirstRun;

		bool _isPlay;

		bool _isDelayStart;

		float _playTime; // 이 파티클의 재생시간을 기록.

		float _delayTime; // 이 파티클을 몇초뒤에 재생 시킬지.

		DUOLMath::Matrix _prevMatrix; // 한프레임동안 생긴 변화량에 대한 TM;
	public:
		// 필살기 Getter
		std::shared_ptr<MuscleGrapics::RenderingData_Particle>& GetParticleData() { return _particleData; }

		// 파티클 이펙트 시작
		void Play();

		// 파티클 이펙트 멈춤
		void Stop();

	public:
		virtual void Start() override;

		virtual void Update() override;

		virtual void Render() override;

	};
}