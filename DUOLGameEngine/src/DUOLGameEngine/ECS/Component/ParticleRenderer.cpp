#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

namespace DUOLGameEngine
{

	ParticleRenderer::ParticleRenderer(const std::weak_ptr<DUOLGameEngine::GameObject>& owner,
		const DUOLCommon::tstring& name) :
		RendererBase(owner, name)
	{
	}

	ParticleRenderer::~ParticleRenderer()
	{
	}

	void ParticleRenderer::OnUpdate(float deltaTime)
	{
		//_particleData->_isDelete = GetGameObject()->GetIsDelete();
		//// ���� ���� ���� ������ ������Ʈ�Ѵ�.
		//if (_isPlay)
		//{
		//	_delayTime += CTime::GetDeltaTime();
		//	if (_delayTime <= _particleData->_commonInfo._startDelay[0])
		//	{
		//		return;
		//	}
		//	_isDelayStart = true;
		//	_playTime += CTime::GetDeltaTime();
		//	_particleData->_commonInfo._transformMatrix = _transform->GetWorldTM();
		//	_particleData->_commonInfo._playTime = _playTime;
		//}
	}

	void ParticleRenderer::OnStart()
	{

	}

	void ParticleRenderer::Render()
	{
		//if (_isFirstRun)
		//	_particleData->_commonInfo._firstRun = false;
		//if (_isPlay && _isDelayStart)
		//{
		//	//MuscleEngine::Get()->GetGraphicsManager()->PostRenderingData_Particle(_particleData); // ��� �׷��Ƚ��� ������ ������ ������! 
		//	
		//	_isFirstRun = true;
		//}
	}

	void ParticleRenderer::Play()
	{
		//_isPlay = true;
		//_isFirstRun = false;
		//_particleData->_commonInfo._firstRun = true;
		//_playTime = 0;
		//_delayTime = 0;
		//_isDelayStart = false;
	}

	void ParticleRenderer::Stop()
	{
		//_isPlay = false;
	}
}
