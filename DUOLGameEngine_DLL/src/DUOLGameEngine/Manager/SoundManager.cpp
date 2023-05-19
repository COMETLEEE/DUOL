#include "DUOLGameEngine/Manager/SoundManager.h"

#include "DUOLGameEngine/ECS/Object/AudioClip.h"

namespace DUOLGameEngine
{
	SoundManager::SoundManager() :
		_totalListenerCount(0)
		, _soundSystem(nullptr)
		, _audioClips{}
	{
		
	}

	SoundManager::~SoundManager()
	{
	}

	void SoundManager::Initialize()
	{
		DUOLSound::SOUNDSYSTEM_SYSTEM_DESC desc;

		_soundSystem = std::make_unique<DUOLSound::SoundSystem>();

		_soundSystem->Initialize(desc);
	}

	void SoundManager::UnInitialize()
	{
		// 소멸자
		_soundSystem.reset();
	}

	void SoundManager::Update()
	{
		// 멀쓰 ..?
		_soundSystem->Update();
	}

	DUOLGameEngine::AudioClip* SoundManager::CreateAudioClip(const DUOLCommon::tstring& filePath, const DUOLCommon::tstring& audioClipName)
	{
		if (_audioClips.contains(audioClipName))
			return _audioClips.at(audioClipName).get();

		DUOLSound::Sound* sound = _soundSystem->GetSound(filePath);

		if (sound == nullptr)
		{
			DUOLSound::SOUNDSYSTEM_SOUND_DESC desc;

			auto str = DUOLCommon::StringHelper::ToString(filePath);

			desc._soundFilePath = str.c_str();

			// TODO : TEST
			desc._flags |= DUOLSound::SOUNDSYSTEM_SOUND_FLAG::SOUND_3D;
			desc._flags |= DUOLSound::SOUNDSYSTEM_SOUND_FLAG::SOUND_LOOP_NORMAL;
			desc._flags |= DUOLSound::SOUNDSYSTEM_SOUND_FLAG::SOUND_3D_LINEARROLLOFF;

			sound = _soundSystem->CreateSound(desc);
		}

		DUOLGameEngine::AudioClip* audioClip = new AudioClip();

		audioClip->SetName(audioClipName);

		audioClip->_sound = sound;

		audioClip->_filePath = filePath;

		_audioClips.insert({ audioClipName, std::unique_ptr<AudioClip>(audioClip) });

		return audioClip;
	}

	void SoundManager::DeleteAudioClip(const DUOLCommon::tstring& audioClipName)
	{
		if (_audioClips.contains(audioClipName))
		{
			auto audioClip = _audioClips.at(audioClipName).get();

			_soundSystem->DeleteSound(audioClip->_filePath);

			_audioClips.erase(audioClipName);
		}
	}

	DUOLGameEngine::AudioClip* SoundManager::GetAudioClip(const DUOLCommon::tstring& audioClipName)
	{
		return _audioClips.contains(audioClipName) ? _audioClips.at(audioClipName).get() : nullptr;
	}

	int SoundManager::AddListener()
	{
		int currentListenerIndex = _totalListenerCount;

		_soundSystem->Set3DListenerCount(++_totalListenerCount);

		return currentListenerIndex;
	}

	void SoundManager::RemoveListener(int index)
	{
		_soundSystem->Set3DListenerCount(--_totalListenerCount);

		// TODO : 모든 현존 오디오 리스너들에게 인덱스 다시 할당해줘야합니다.
	}

	void SoundManager::Get3DListenerAttribute(DUOLSound::ListenerAttribute& outListenerAttribute)
	{
		_soundSystem->Get3DListenerAttribute(outListenerAttribute);
	}

	void SoundManager::Set3DListenerAttribute(const DUOLSound::ListenerAttribute& listenerAttribute)
	{
		_soundSystem->Set3DListenerAttribute(listenerAttribute);
	}
}