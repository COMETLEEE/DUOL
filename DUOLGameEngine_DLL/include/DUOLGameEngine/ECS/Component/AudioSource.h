#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

#include "DUOLSound/Sound.h"
#include "DUOLSound/Channel.h"

namespace DUOLGameEngine
{
	class AudioClip;
}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API AudioSource : public DUOLGameEngine::BehaviourBase
	{
	public:
		AudioSource(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("AudioSource"));

		virtual ~AudioSource() override;

	private:
		DUOLGameEngine::AudioClip* _audioClip;

		DUOLSound::Channel _channel;

		bool _playOnAwake;

#pragma region FOR_REFLECTION_GETTER
	private:
		float GetterMinDistance() const;

		float GetterMaxDistance() const;

		float GetterVolume() const;

		bool GetterMute() const;

		bool GetterIsLoop();

		bool GetterIsPlaying() const;
#pragma endregion

	public:
		void GetMaxDistance(float& outMaxDistance) const;

		void SetMaxDistance(float maxDistance);

		void GetMinDistance(float& outMinDistance) const;

		void SetMinDistance(float minDistance);

		void GetVolume(float& outVolume) const;

		void SetVolume(float volume);

		void GetMute(bool& outMute) const;

		void SetMute(bool value);

		void GetIsLoop(bool& outLoop);

		void SetIsLoop(bool value);

		void IsPlaying(bool& outIsPlaying) const;

		DUOLGameEngine::AudioClip* GetAudioClip() const;

		void SetAudioClip(DUOLGameEngine::AudioClip* audioClip);
		
		void Stop();

		void Pause();

		void UnPause();

		void Play();

		bool GetPlayOnAwake() const;

		void SetPlayOnAwake(bool value);

		virtual void OnAwake() override;

		virtual void OnLateUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};
}