#include "pch.h"
#include "ParticleRenderer.h"

namespace Muscle
{
	ParticleRenderer::ParticleRenderer(std::shared_ptr<GameObject> _GameObject) : IComponents(_GameObject),
		_isFirstUpdate(false)
	{
		_particleData = std::make_shared<RenderingData_Particle>();

		_isPlay = false;

		_offsetMatrix = Matrix::Identity;

		// �׽�Ʈ ���� ���ؼ� ��� �ʱ�ȭ ^__^
		_particleData->_initInfo->_firstRun = true;
		_particleData->_initInfo->_isLoop = true;
		_particleData->_initInfo->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;
		_particleData->_initInfo->_maxParticleCount = 1000;
		_particleData->_initInfo->_particlePlayID = 0;

		_particleData->_shapeInfo->_emitVelocity = Vector3::Up;
		_particleData->_shapeInfo->_emitTime = 0.002f;
		_particleData->_shapeInfo->_radius = 1.f;
		_particleData->_shapeInfo->_particleAverageSize = Vector2(0.1f, 0.1f);
		_particleData->_shapeInfo->_period = 0.2f;

		_particleData->_shapeInfo->_acceleration = Vector3(0.f, 3.f, 0.f);
		_particleData->_shapeInfo->_lifeSpan = 1.5f;
	}

	ParticleRenderer::~ParticleRenderer()
	{
		_transform.reset();

		_particleData.reset();
	}

	void ParticleRenderer::Play()
	{
		// �����մϴ�.
		_isPlay = true;

		_particleData->_initInfo->_firstRun = true;
	}

	void ParticleRenderer::Stop()
	{
		_isPlay = false;
	}

	void ParticleRenderer::Start()
	{
		_transform = GetGameObject()->GetTransform();
	}

	void ParticleRenderer::Update()
	{
		// ���� ���� ���� ������ ������Ʈ�Ѵ�.
		if (_isPlay)
		{
			// World Transform Matrix (��� ����, ���� ���� �ɼ��� �༭ ��ƼŬ�� ��Ʈ�� �����ϰ� �� ��
			// �ִ�. (In Unity)

			// Offset��ŭ ��ȯ �� Ʈ�������� �°� �׸���. ������Ʈ�� �������� �����Ѵ�. (radius�� �ִ�.)
			_particleData->_shapeInfo->_transformMatrix = _offsetMatrix * _transform->GetWorldScaleTM().Invert() * _transform->GetWorldTM();

			if (!_isFirstUpdate)
				_isFirstUpdate = true;
		}
	}

	void ParticleRenderer::Render()
	{
		if (!_isFirstUpdate)
			return;

		// ���� �� �� ù ��° ������Ʈ�� ����ǰ� ������ �ǽ��Ѵ�.
		if (_isPlay)
		{
			// ������ �ƴϸ� ���߿� ���� �� ���� ..!
			if (!_particleData->_initInfo->_isLoop)
			{
				MuscleEngine::Get()->GetGraphicsManager()->PostRenderingData_Particle(_particleData);

				Stop();
			}
			else
				MuscleEngine::Get()->GetGraphicsManager()->PostRenderingData_Particle(_particleData);
		}
	}
}