#include "DUOLGameEngine/ECS/Component/AudioListener.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/SoundManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::AudioListener>("AudioListener")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLGameEngine
{
	AudioListener::AudioListener(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _listenerAttribute()
	{
		
	}

	AudioListener::~AudioListener()
	{

	}

	void AudioListener::OnEnable()
	{
		BehaviourBase::OnEnable();

		_listenerAttribute._index = DUOLGameEngine::SoundManager::GetInstance()->AddListener();
	}

	void AudioListener::OnDisable()
	{
		BehaviourBase::OnDisable();

		DUOLGameEngine::SoundManager::GetInstance()->RemoveListener(_listenerAttribute._index);
	}

	void AudioListener::OnLateUpdate(float deltaTime)
	{
		BehaviourBase::OnLateUpdate(deltaTime);

		_listenerAttribute._pos = GetTransform()->GetWorldPosition();

		_listenerAttribute._velocity = DUOLMath::Vector3::Zero;

		_listenerAttribute._forward = GetTransform()->GetLook();

		_listenerAttribute._up = GetTransform()->GetUp();

		DUOLGameEngine::SoundManager::GetInstance()->Set3DListenerAttribute(_listenerAttribute);
	}
}