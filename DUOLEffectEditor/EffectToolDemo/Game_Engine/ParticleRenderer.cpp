#include "pch.h"
#include "ParticleRenderer.h"

namespace Muscle
{
	ParticleRenderer::ParticleRenderer(std::shared_ptr<GameObject> _GameObject) : IComponents(_GameObject)
	{
		_particleData = std::make_shared<RenderingData_Particle>();

		_isPlay = false;

		_isFirstRun = false;
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
	}

	void ParticleRenderer::Stop()
	{
		_isPlay = false;
	}

	void ParticleRenderer::Start()
	{
		_transform = GetGameObject()->GetTransform();
		_particleData->_commonInfo->_refTextureID = IGameEngine::Get()->GetGraphicsManager()->GetTexture(TEXT("test1"));
		Play();
	}

	void ParticleRenderer::Update()
	{
		// ���� ���� ���� ������ ������Ʈ�Ѵ�.
		if (_isPlay)
		{
			_particleData->_commonInfo->_transformMatrix = _transform->GetWorldTM();
		}
	}

	void ParticleRenderer::Render()
	{
		if (_isFirstRun)
			_particleData->_commonInfo->_firstRun = false;
		if (_isPlay)
		{
			MuscleEngine::Get()->GetGraphicsManager()->PostRenderingData_Particle(_particleData);

			_isFirstRun = true;
		}
	}
}