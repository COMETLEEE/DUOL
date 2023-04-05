#include "DUOLGameEngine/ECS/Component/AudioSource.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"

#include "DUOLGameEngine/ECS/Object/AudioClip.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::AudioSource>("AudioSource")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("AudioClip", &DUOLGameEngine::AudioSource::GetAudioClip, &DUOLGameEngine::AudioSource::SetAudioClip)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByString, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::Resource)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::AudioClip)
	);
}

namespace DUOLGameEngine
{
	AudioSource::AudioSource(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _audioClip(nullptr)
		, _channel(nullptr)
	{
	}

	AudioSource::~AudioSource()
	{

	}

	void AudioSource::GetMaxDistance(float& outMaxDistance) const
	{
		if (_audioClip == nullptr)
			return;

		_channel.GetMaxDistance(outMaxDistance);
	}

	void AudioSource::SetMaxDistance(float maxDistance)
	{
		if (_audioClip == nullptr)
			return;

		_channel.SetMaxDistance(maxDistance);
	}

	void AudioSource::GetMinDistance(float& outMinDistance) const
	{
		if (_audioClip == nullptr)
			return;

		_channel.GetMinDistance(outMinDistance);
	}

	void AudioSource::SetMinDistance(float minDistance)
	{
		if (_audioClip == nullptr)
			return;

		_channel.SetMinDistance(minDistance);
	}

	void AudioSource::GetVolume(float& outVolume) const
	{
		if (_audioClip == nullptr)
			return;

		_channel.GetVolume(outVolume);
	}

	void AudioSource::SetVolume(float volume)
	{
		if (_audioClip == nullptr)
			return;

		_channel.SetVolume(volume);
	}

	void AudioSource::GetMute(bool& outMute) const
	{
		if (_audioClip == nullptr)
			return;

		_channel.GetMute(outMute);
	}

	void AudioSource::SetMute(bool value)
	{
		if (_audioClip == nullptr)
			return;

		_channel.SetMute(value);
	}

	void AudioSource::GetIsLoop(bool& outLoop)
	{
		if (_audioClip == nullptr)
			return;

		_channel.GetIsLoop(outLoop);
	}

	void AudioSource::SetIsLoop(bool value)
	{
		if (_audioClip == nullptr)
			return;

		_channel.SetIsLoop(value);
	}

	void AudioSource::IsPlaying(bool& outIsPlaying)
	{
		if (_audioClip == nullptr)
			return;

		_channel.IsPlaying(outIsPlaying);
	}

	DUOLGameEngine::AudioClip* AudioSource::GetAudioClip() const
	{
		return _audioClip;
	}

	void AudioSource::SetAudioClip(DUOLGameEngine::AudioClip* audioClip)
	{
		if (audioClip == nullptr)
		{
			_audioClip = nullptr;

			_channel = DUOLSound::Channel(nullptr);
		}
		else
		{
			_audioClip = audioClip;

			_channel = _audioClip->_sound->PlaySound(false);
		}
	}

	void AudioSource::OnLateUpdate(float deltaTime)
	{
		if (_audioClip != nullptr)
		{
			_channel.Set3DAttribute(GetTransform()->GetWorldPosition(), DUOLMath::Vector3::Zero);
		}
	}
}