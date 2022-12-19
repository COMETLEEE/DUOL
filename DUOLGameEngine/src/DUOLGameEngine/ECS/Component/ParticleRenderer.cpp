#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"

namespace DUOLGameEngine
{

	ParticleRenderer::ParticleRenderer(const std::weak_ptr<DUOLGameEngine::GameObject>& owner,
		const DUOLCommon::tstring& name) :
		RendererBase(owner, name)
	{
		_renderObjectInfo._renderInfo = &_particleInfo;
	}

	ParticleRenderer::~ParticleRenderer()
	{
	}

	void ParticleRenderer::OnUpdate(float deltaTime)
	{

		//_particleInfo._particleData._isDelete = GetGameObject()->GetIsDelete(); // ������ �׷��Ƚ� ���������� ���۸� ������ �� Ȯ�� �뵵 ������, ��� �׷��Ƚ��� ��� ���� �𸣰ڴ�.
		// ���� ���� ���� ������ ������Ʈ�Ѵ�.
		if (_isPlay)
		{
			_delayTime += deltaTime;
			if (_delayTime <= _particleInfo._particleData._commonInfo._startDelay[0])
				return;

			_isDelayStart = true;

			_playTime += deltaTime;

			_particleInfo._particleData._commonInfo._transformMatrix = GetTransform()->GetWorldMatrix();

			_particleInfo._particleData._commonInfo._playTime = _playTime;
		}
	}

	void ParticleRenderer::OnStart()
	{

	}

	void ParticleRenderer::Render()
	{
		if (_isFirstRun)
			_particleInfo._particleData._commonInfo._firstRun = false;

		if (_isPlay && _isDelayStart)
		{
			//MuscleEngine::Get()->GetGraphicsManager()->PostRenderingData_Particle(_particleData); // ��� �׷��Ƚ��� ������ ������ �������Ѵ�! 

			_isFirstRun = true;
		}
	}

	void ParticleRenderer::Play()
	{
		_isPlay = true;

		_isFirstRun = false;

		_particleInfo._particleData._commonInfo._firstRun = true;

		_playTime = 0;

		_delayTime = 0;

		_isDelayStart = false;
	}

	void ParticleRenderer::Stop()
	{
		_isPlay = false;
	}

	DUOLGraphicsEngine::RenderingData_Particle& ParticleRenderer::GetParticleData()
	{
		return _particleInfo._particleData;
	}
}
