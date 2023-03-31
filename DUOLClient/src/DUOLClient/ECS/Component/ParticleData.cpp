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

	void ParticleData::OnStart()
	{
		auto temp = GetGameObject()->GetComponent<DUOLGameEngine::ParticleRenderer>();

		if (temp)
			_particleRenderer = temp;
		else
		{
			// ���ܹ߻�.
			// �̹� ������ ��ƼŬ �������� �־�� �����ϴ�.
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
		_isOnTimer = true;

		_timer = time;
	}

	void ParticleData::ResetData()
	{
		_isOnTimer = false;

		_timer = 0;
	}

	bool ParticleData::CheckCanReturnQueue()
	{
		if (_isOnTimer && _timer <= 0)
			return true;
		else if (!GetGameObject()->GetIsActive())
			return true;
		else if (!_particleRenderer->GetIsPlay())
			return true;
		else
			return false;
	}
}
