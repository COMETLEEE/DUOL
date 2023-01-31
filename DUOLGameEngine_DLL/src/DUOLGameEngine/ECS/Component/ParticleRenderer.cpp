#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"

namespace DUOLGameEngine
{

	ParticleRenderer::ParticleRenderer(const std::weak_ptr<DUOLGameEngine::GameObject>& owner,
		const DUOLCommon::tstring& name) :
		RendererBase(owner, name)
		, _particleInfo(),
		_prevMatrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1)
	{

		_renderObjectInfo._renderInfo = &_particleInfo;

		_isPlay = false;

		_isDelayStart = false;

		_isFirstRun = false;

		_particleInfo._objectID = GetGameObject()->GetUUID();
	}

	ParticleRenderer::~ParticleRenderer()
	{
	}

	void ParticleRenderer::OnUpdate(float deltaTime)
	{

		//_particleInfo._particleData._isDelete = GetGameObject()->GetIsDelete(); // 원래는 그래픽스 엔진에서도 버퍼를 삭제할 때 확인 용도 였지만, 경민 그래픽스는 어떻게 할지 모르겠다.
		// 실행 중일 때만 정보를 업데이트한다.
		if (_isPlay)
		{
			_delayTime += deltaTime;
			if (_delayTime <= _particleInfo._particleData._commonInfo._startDelay[0])
				return;

			_isDelayStart = true;

			_playTime += deltaTime;

			_particleInfo._particleData._commonInfo._transformMatrix = GetTransform()->GetWorldMatrix();

			_particleInfo._particleData._commonInfo._deltaMatrix = _prevMatrix.Invert() * GetTransform()->GetWorldMatrix();

			_particleInfo._particleData._commonInfo._playTime = _playTime;

			_prevMatrix = GetTransform()->GetWorldMatrix();
		}
	}

	void ParticleRenderer::OnStart()
	{
		CreateParticleBuffer();
	}

	void ParticleRenderer::Render()
	{
		if (_isFirstRun)
			_particleInfo._particleData._commonInfo._firstRun = false;

		if (_isPlay && _isDelayStart)
		{
			GraphicsManager::GetInstance()->ReserveRenderObject(&_renderObjectInfo);

			_isFirstRun = true;
		}
	}

	void ParticleRenderer::Play()
	{
		Stop();

		_isPlay = true;

		_isFirstRun = false;

		_particleInfo._particleData._commonInfo._firstRun = true;

		_playTime = 0;

		_delayTime = 0;

		_isDelayStart = false;

		for (auto iter : GetGameObject()->GetTransform()->GetChildGameObjects())
		{
			auto renderer = iter->GetComponent<ParticleRenderer>();
			if (renderer)
				renderer->Play();
		}

	}

	void ParticleRenderer::Stop()
	{
		_isPlay = false;
		for (auto iter : GetGameObject()->GetTransform()->GetChildGameObjects())
		{
			auto renderer = iter->GetComponent<ParticleRenderer>();
			if (renderer)
				renderer->Stop();
		}
	}

	DUOLGraphicsEngine::RenderingData_Particle& ParticleRenderer::GetParticleData()
	{
		return _particleInfo._particleData;
	}

	void ParticleRenderer::CreateParticleBuffer()
	{
		const auto& _graphicsEngine = DUOLGameEngine::GraphicsManager::GetInstance()->_graphicsEngine;
		auto gameObject = GetGameObject();
		auto uuid = gameObject->GetUUID();
		DUOLCommon::tstring str =  _T("Particle") + DUOLCommon::StringHelper::ToTString(uuid);
		_renderObjectInfo._mesh = _graphicsEngine->CreateParticle(
			str,
			_particleInfo._particleData._commonInfo._maxParticles,
			_particleInfo._particleData._emission._emissiveCount
		);
		_renderObjectInfo._materials = &_primitiveMaterials;
	}
}
