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

		float _playTime; // �� ��ƼŬ�� ����ð��� ���.

		float _delayTime; // �� ��ƼŬ�� ���ʵڿ� ��� ��ų��.

		std::list<DUOLMath::Matrix> _prevMatrix;
	public:
		// �ʻ�� Getter
		std::shared_ptr<MuscleGrapics::RenderingData_Particle>& GetParticleData() { return _particleData; }

		// ��ƼŬ ����Ʈ ����
		void Play();

		// ��ƼŬ ����Ʈ ����
		void Stop();

	public:
		virtual void Start() override;

		virtual void Update() override;

		virtual void Render() override;

	};
}