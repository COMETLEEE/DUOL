#include "DUOLClient/Manager/EnemyAudioManager.h"
namespace DUOLClient
{
	void EnemyAudioManager::Initialize()
	{
		// 오디오 클립을 미리 가져와서 저장한다.
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
