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
	)
	.property("Min Distance", &DUOLGameEngine::AudioSource::GetterMinDistance, &DUOLGameEngine::AudioSource::SetMinDistance)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("Max Distance", &DUOLGameEngine::AudioSource::GetterMaxDistance, &DUOLGameEngine::AudioSource::SetMaxDistance)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("Volume", &DUOLGameEngine::AudioSource::GetterVolume, &DUOLGameEngine::AudioSource::SetVolume)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("Mute", &DUOLGameEngine::AudioSource::GetterMute, &DUOLGameEngine::AudioSource::SetMute)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("Loop", &DUOLGameEngine::AudioSource::GetterIsLoop, &DUOLGameEngine::AudioSource::SetIsLoop)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("PlayOnAwake", &DUOLGameEngine::AudioSource::GetPlayOnAwake, &DUOLGameEngine::AudioSource::SetPlayOnAwake)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property_readonly("IsPlaying", &DUOLGameEngine::AudioSource::GetterIsPlaying)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	);
}

namespace DUOLGameEngine
{
	AudioSource::AudioSource(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _audioClip(nullptr)
		, _channel(nullptr)
		, _playOnAwake(true)
		, _minDistance(0.0f)
		, _maxDistnace(1000.0f)
		, _isLoop(true)
		, _volume(1.0f)
		, _mute(false)
	{
	}

	AudioSource::~AudioSource()
	{
		// 사라지기 전에 한 번 체크 ..!
		if (GetterIsPlaying())
			Stop();
	}

	float AudioSource::GetterMinDistance() const
	{
		float ret;

		GetMinDistance(ret);

		return ret;
	}

	float AudioSource::GetterMaxDistance() const
	{
		float ret;

		GetMaxDistance(ret);

		return ret;
	}

	float AudioSource::GetterVolume() const
	{
		float ret;

		GetVolume(ret);

		return ret;
	}

	bool AudioSource::GetterMute() const
	{
		bool mute;

		GetMute(mute);

		return mute;
	}

	bool AudioSource::GetterIsLoop()
	{
		bool loop;

		GetIsLoop(loop);

		return loop;
	}

	bool AudioSource::GetterIsPlaying() const
	{
		bool isPlaying;

		IsPlaying(isPlaying);

		return isPlaying;
	}

	void AudioSource::SetAllData()
	{
		_channel.SetIsLoop(_isLoop);

		_channel.SetMinDistance(_minDistance);

		_channel.SetMaxDistance(_maxDistnace);

		_channel.SetVolume(_volume);

		_channel.SetMute(_mute);
	}

	void AudioSource::GetMaxDistance(float& outMaxDistance) const
	{
		outMaxDistance = _maxDistnace;
	}

	void AudioSource::SetMaxDistance(float maxDistance)
	{
		_maxDistnace = maxDistance;

		if (_audioClip == nullptr)
			return;

		_channel.SetMaxDistance(maxDistance);
	}

	void AudioSource::GetMinDistance(float& outMinDistance) const
	{
		outMinDistance = _minDistance;
	}

	void AudioSource::SetMinDistance(float minDistance)
	{
		_minDistance = minDistance;

		if (_audioClip == nullptr)
			return;

		_channel.SetMinDistance(minDistance);
	}

	void AudioSource::GetVolume(float& outVolume) const
	{
		outVolume = _volume;
	}

	void AudioSource::SetVolume(float volume)
	{
		_volume = volume;

		if (_audioClip == nullptr)
			return;

		_channel.SetVolume(std::clamp<float>(volume, 0.f, 1.f));
	}

	void AudioSource::GetMute(bool& outMute) const
	{
		outMute = _mute;
	}

	void AudioSource::SetMute(bool value)
	{
		_mute = value;

		if (_audioClip == nullptr)
			return;

		_channel.SetMute(value);
	}

	void AudioSource::GetIsLoop(bool& outLoop)
	{
		outLoop = _isLoop;
	}

	void AudioSource::SetIsLoop(bool value)
	{
		_isLoop = value;

		if (_audioClip == nullptr)
			return;

		_channel.SetIsLoop(value);
	}

	void AudioSource::IsPlaying(bool& outIsPlaying) const
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
			// 기존에 플레이 중인 오디오 클립이 있다면 .. 정지 및 반환
			if (GetterIsPlaying() == true)
				_channel.Stop();

			_audioClip = audioClip;

			_channel = _audioClip->CreateChannel(true);

			SetAllData();
		}
	}

	void AudioSource::Stop()
	{
		if (_audioClip != nullptr)
			_channel.Stop();
	}

	void AudioSource::Pause()
	{
		if (_audioClip != nullptr)
			_channel.Pause();
	}

	void AudioSource::UnPause()
	{
		if (_audioClip != nullptr)
			_channel.UnPause();
	}

	void AudioSource::Play()
	{
		if (_audioClip != nullptr)
		{
			if (GetterIsPlaying())
				_channel.Stop();

			_channel = _audioClip->CreateChannel(false);

			SetAllData();
		}
	}

	bool AudioSource::GetPlayOnAwake() const
	{
		return _playOnAwake;
	}

	void AudioSource::SetPlayOnAwake(bool value)
	{
		_playOnAwake = value;
	}

	void AudioSource::OnAwake()
	{
		BehaviourBase::OnAwake();

		if (_audioClip != nullptr)
		{
			// Set All Properties.w
			SetAllData();
		}

		if ((_audioClip != nullptr) && (_playOnAwake))
		{
			_channel.UnPause();
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