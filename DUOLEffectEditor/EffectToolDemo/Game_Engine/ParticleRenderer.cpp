#include "pch.h"
#include "ParticleRenderer.h"

namespace Muscle
{
	ParticleRenderer::ParticleRenderer(std::shared_ptr<GameObject> _GameObject) : IComponents(_GameObject),
		_isFirstUpdate(false)
	{
		_particleData = std::make_shared<RenderingData_Particle>();

		_isPlay = false;

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
		Play();
	}

	void ParticleRenderer::Update()
	{
		// ���� ���� ���� ������ ������Ʈ�Ѵ�.
		if (_isPlay)
		{
			// World Transform Matrix (��� ����, ���� ���� �ɼ��� �༭ ��ƼŬ�� ��Ʈ�� �����ϰ� �� ��
			// �ִ�. (In Unity)

			// Offset��ŭ ��ȯ �� Ʈ�������� �°� �׸���. ������Ʈ�� �������� �����Ѵ�. (radius�� �ִ�.)
			_particleData->_commonInfo->_transformMatrix = _transform->GetWorldTM();

			if (!_isFirstUpdate)
				_isFirstUpdate = true;
		}
	}

	void ParticleRenderer::Render()
	{
		if (!_isFirstUpdate)
			return;

		if (_isPlay)
		{
			MuscleEngine::Get()->GetGraphicsManager()->PostRenderingData_Particle(_particleData);
		}
	}
}