#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"

namespace DUOLGameEngine
{

	ParticleRenderer::ParticleRenderer(const std::weak_ptr<DUOLGameEngine::GameObject>& owner,
		const DUOLCommon::tstring& name) :
		RendererBase(owner, name)
		, _particleInfo()
	{
		const auto& _graphicsEngine = DUOLGameEngine::GraphicsManager::GetInstance()->_graphicsEngine;

		_renderObjectInfo._renderInfo = &_particleInfo;
		_renderObjectInfo._mesh = _graphicsEngine->CreateParticle(
			_T("Particle"),
			_particleInfo._particleData._commonInfo._maxParticles
		);
		_renderObjectInfo._materials = &_primitiveMaterials;
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
			GraphicsManager::GetInstance()->ReserveRenderObject(_renderObjectInfo);

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

	void ParticleRenderer::CreateParticleBuffer()
	{



	}
}
