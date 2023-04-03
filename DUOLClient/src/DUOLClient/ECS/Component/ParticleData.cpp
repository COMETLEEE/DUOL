#include "DUOLClient/ECS/Component/ParticleData.h"
#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::ParticleData>("ParticleData")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);

}

namespace DUOLClient
{
	DUOLClient::ParticleData::ParticleData(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		MonoBehaviourBase(owner, name), _isOnTimer(false), _timer(0)
	{
	}

	ParticleData::~ParticleData()
	{
	}

	void ParticleData::OnAwake()
	{
		auto temp = GetGameObject()->GetComponent<DUOLGameEngine::ParticleRenderer>();

		if (temp)
			_particleRenderer = temp;
		else
		{
			// 예외발생.
			// 이미 생성된 파티클 렌더러가 있어야 가능하다.
		}
	}

	void ParticleData::OnUpdate(float deltaTime)
	{
		if (_isOnTimer)
		{
			_timer -= deltaTime;
		}
	}

	void ParticleData::SetTimer(float time)
	{
		if (time != std::numeric_limits<float>::max())
			_isOnTimer = true;

		_timer = time;
	}

	void ParticleData::ResetData()
	{
		_isOnTimer = false;

		_timer = 0;
	}

	bool ParticleData::CheckCanReturnQueue() const
	{
		if (_isOnTimer && _timer <= 0)
			return true;
		if (!GetGameObject()->GetIsActive())
			return true;
		if (!_particleRenderer->GetIsPlay())
			return true;

		return false;
	}
}
