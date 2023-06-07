#include "DUOLClient/Manager/EnemyAudioManager.h"
namespace DUOLClient
{
	void EnemyAudioManager::Initialize()
	{
		// ����� Ŭ���� �̸� �����ͼ� �����Ѵ�.
	}

	DUOLGameEngine::AudioClip* EnemyAudioManager::GetAudioClip(EnemyAudioEnum enemySoundEnum)
	{
		if (_audioClipsCount[enemySoundEnum] > 0)
		{
			_audioClipsCount[enemySoundEnum]--;
			return _audioClips[enemySoundEnum];
		}
		return nullptr;
	}

	void EnemyAudioManager::ReturnAudioClip(EnemyAudioEnum enemySoundEnum)
	{
		_audioClipsCount[enemySoundEnum]++;
	}
}
