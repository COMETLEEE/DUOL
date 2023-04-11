#include "ParticleRenderer.h"
#include "CTime.h"
#include "IGameEngine.h"
#include "GraphicsManager.h"
#include "Transform.h"
#include "MuscleEngine.h"
#include <limits>

namespace Muscle
{
	ParticleRenderer::ParticleRenderer(std::shared_ptr<GameObject> _GameObject) : IComponents(_GameObject),
		_playTime(0),
		_delayTime(0),
		_prevMatrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1)
	{
		_particleData = std::make_shared<MuscleGrapics::RenderingData_Particle>();

		_particleData->_renderer._texturePath = TEXT("Asset/Particle/Resource\\Image\\test1.png");

		_particleData->_renderer._traillTexturePath = TEXT("Asset/Particle/Resource\\Image\\test1.png");

		_isPlay = false;

		_isDelayStart = false;

		_isFirstRun = false;

		_particleData->_objectID = _GameObject->GetObjectID();

		//_particleData->shaderName.push_back(TEXT("BasicParticle"));
		_particleData->shaderName.push_back("OITParticlePass");

		_particleData->shaderName.push_back("BasicParticleObjectID");


	}

	ParticleRenderer::~ParticleRenderer()
	{
		_transform.reset();

		_particleData.reset();
	}

	void ParticleRenderer::Play()
	{
		Stop();
		// 시작합니다.
		_isPlay = true;
		_isFirstRun = false;
		_particleData->_commonInfo._firstRun = true;
		_playTime = 0;
		_delayTime = 0;
		_isDelayStart = false;

		for (auto iter : GetGameObject()->GetChildrens())
		{
			auto renderer = iter->GetComponent<ParticleRenderer>();
			if (renderer)
				renderer->Play();
		}

	}

	void ParticleRenderer::Stop()
	{
		_isPlay = false;
		for (auto iter : GetGameObject()->GetChildrens())
		{
			auto renderer = iter->GetComponent<ParticleRenderer>();
			if (renderer)
				renderer->Stop();
		}
		_particleData->_particleCount = 0;
	}

	void ParticleRenderer::Start()
	{
		_transform = GetGameObject()->GetTransform();
		Play();
	}

	void ParticleRenderer::Update()
	{
		_particleData->_isDelete = GetGameObject()->GetIsDelete();
		// 실행 중일 때만 정보를 업데이트한다.
		if (_isPlay)
		{
			float deltaTime = CTime::GetDeltaTime();

			if (_particleData->_emission._emissiveTimer >= _particleData->_emission._emissiveTime)
				_particleData->_emission._emissiveTimer = 0;

			_particleData->_emission._emissiveTimer += deltaTime * _particleData->_commonInfo._simulationSpeed;

			_delayTime += deltaTime;
			if (_delayTime <= _particleData->_commonInfo._startDelay[0])
			{
				return;
			}
			_isDelayStart = true;
			_playTime += deltaTime;
			_particleData->_commonInfo._transformMatrix = _transform->GetWorldTM();

			if (_particleData->_commonInfo._transformMatrix != _prevMatrix)
				_particleData->_commonInfo._deltaMatrix = _prevMatrix.Invert() * _transform->GetWorldTM();
			else
				_particleData->_commonInfo._deltaMatrix = DUOLMath::Matrix::Identity;

			_particleData->_commonInfo._playTime = _playTime;

			_prevMatrix = _transform->GetWorldTM();
		}
	}

	void ParticleRenderer::Render()
	{
		if (_isFirstRun)
			_particleData->_commonInfo._firstRun = false;
		else
			_particleData->_emission._emissiveTimer = (std::numeric_limits<float>::max)();

		if (_isPlay && _isDelayStart)
		{
			MuscleEngine::Get()->GetGraphicsManager()->PostRenderingData_Particle(_particleData);

			_isFirstRun = true;
		}
	}
}