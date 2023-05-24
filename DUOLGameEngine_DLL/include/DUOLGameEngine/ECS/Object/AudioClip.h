#pragma once
#include "DUOLGameEngine/ECS/ObjectBase.h"

#include "DUOLGameEngine/Manager/SoundManager.h"

namespace DUOLSound
{
	class Channel;
	class Sound;
}

namespace DUOLGameEngine
{
	/**
	 * \brief ����� ��� ����. ������ �����͸� ��� �ִ� ������ ������.
	 */
	class AudioClip : public DUOLGameEngine::ObjectBase
	{
	public:
		AudioClip(const DUOLCommon::tstring& name = TEXT("AudioClip"));

		virtual ~AudioClip() override;

	private:
		DUOLSound::Sound*		_sound;

		DUOLCommon::tstring		_filePath;

	public:
		DUOLSound::Channel CreateChannel(bool paused = false);

		// ���� �����ϴ� ..!
		void SetLoopOff();

		void SetLoopOn();

		void Set2DSound();

		void Set3DSound();

		RTTR_ENABLE(DUOLGameEngine::ObjectBase)
		
#pragma region FRIEND_CLASS
		friend class SoundManager;

		friend class AudioSource;
#pragma endregion
	};
}