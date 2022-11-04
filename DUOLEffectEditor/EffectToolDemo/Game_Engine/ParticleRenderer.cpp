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

		// 테스트 용을 위해서 잠깐 초기화 ^__^
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
		// 시작합니다.
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
		// 실행 중일 때만 정보를 업데이트한다.
		if (_isPlay)
		{
			// World Transform Matrix (사실 로컬, 월드 등의 옵션을 줘서 파티클을 컨트롤 가능하게 할 수
			// 있다. (In Unity)

			// Offset만큼 변환 후 트랜스폼에 맞게 그린다. 오브젝트의 스케일은 제외한다. (radius가 있다.)
			_particleData->_shapeInfo->_transformMatrix = _offsetMatrix * _transform->GetWorldScaleTM().Invert() * _transform->GetWorldTM();

			if (!_isFirstUpdate)
				_isFirstUpdate = true;
		}
	}

	void ParticleRenderer::Render()
	{
		if (!_isFirstUpdate)
			return;

		// 실행 중 및 첫 번째 업데이트가 진행되고 나서만 실시한다.
		if (_isPlay)
		{
			// 루프가 아니면 도중에 끊을 수 없다 ..!
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