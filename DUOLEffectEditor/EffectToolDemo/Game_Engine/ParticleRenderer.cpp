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
		// 시작합니다.
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
		// 실행 중일 때만 정보를 업데이트한다.
		if (_isPlay)
		{
			// World Transform Matrix (사실 로컬, 월드 등의 옵션을 줘서 파티클을 컨트롤 가능하게 할 수
			// 있다. (In Unity)

			// Offset만큼 변환 후 트랜스폼에 맞게 그린다. 오브젝트의 스케일은 제외한다. (radius가 있다.)
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