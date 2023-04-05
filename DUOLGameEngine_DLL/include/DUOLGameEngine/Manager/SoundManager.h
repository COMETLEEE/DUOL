#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLSound/SoundSystem.h"

namespace DUOLGameEngine
{
	class AudioClip;
}

namespace DUOLGameEngine
{
	/**
	 * \brief 사운드 출력 관련 매니저
	 */
	class DUOL_GAMEENGINE_API SoundManager : public DUOLGameEngine::SingletonBase<SoundManager>
	{
		DECLARE_SINGLETON(SoundManager)

		DELETE_COPY_MOVE(SoundManager)

	public:
		virtual ~SoundManager() override;

	private:
		int _totalListenerCount;

		std::unique_ptr<DUOLSound::SoundSystem> _soundSystem;

		std::unordered_map<DUOLCommon::tstring, std::unique_ptr<DUOLGameEngine::AudioClip>> _audioClips;

	public:
		void Initialize();

		void UnInitialize();

		void Update();

		DUOLGameEngine::AudioClip* CreateAudioClip(const DUOLCommon::tstring& filePath, const DUOLCommon::tstring& audioClipName);

		void DeleteAudioClip(const DUOLCommon::tstring& audioClipName);

		DUOLGameEngine::AudioClip* GetAudioClip(const DUOLCommon::tstring& audioClipName);

		int AddListener();

		void RemoveListener(int index);

		void Get3DListenerAttribute(DUOLSound::ListenerAttribute& outListenerAttribute);
		
		void Set3DListenerAttribute(const DUOLSound::ListenerAttribute& listenerAttribute);

		friend class ResourceManager;
	};
}