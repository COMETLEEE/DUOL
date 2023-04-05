#pragma once
#include "DUOLGameEngine/ECS/ObjectBase.h"

#include "DUOLGameEngine/Manager/SoundManager.h"

namespace DUOLSound
{
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
		// ���� �����ϴ� ..!



		RTTR_ENABLE(DUOLGameEngine::ObjectBase)
		
#pragma region FRIEND_CLASS
		friend class SoundManager;

		friend class AudioSource;
#pragma endregion
	};
}