#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Object/AudioClip.h"
#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"

namespace DUOLGameEngine
{
	class Animator;
}

/**
 * \brief ������� ���� BossMonster Script�Դϴ�.
 */
namespace DUOLClient
{
class BossMonsterScript : public DUOLGameEngine::MonoBehaviourBase
{
public:
	BossMonsterScript(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("BossMonsterScript"));

	virtual ~BossMonsterScript() override;

public:
/**
* \brief ���� ������ �� OnStart ���� ������ ȣ��˴ϴ�.
* �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
*/
	virtual void OnAwake();

	/**
	 * \brief ���� ������ �� OnAwake ���Ŀ� ȣ��˴ϴ�.
	 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
	 */
	virtual void OnStart();

	/**
	 * \brief ������Ʈ�� �ı��� �� ȣ��˴ϴ�.
	 */
	virtual void OnDestroy();

	/**
	 * \brief �� �����ӿ� ȣ��˴ϴ�.
	 * \param deltaTime ������ �� �ð� �����Դϴ�.
	 */
	virtual void OnUpdate(float deltaTime);

	DUOLGameEngine::CoroutineHandler AnimationStart();

public:
	DUOLGameEngine::GameObject* _cachedPlayerDummy;

	DUOLGameEngine::Animator* _cachedPlayerAnimator;

	DUOLGameEngine::AudioSource* _cachedPlayerAudioSource;

	DUOLGameEngine::GameObject* _cachedBossDummy;

	DUOLGameEngine::Animator* _cachedBossAnimator;

	DUOLGameEngine::AudioClip* _bossVoice1;

	DUOLGameEngine::AudioClip* _bossVoice2;

	DUOLGameEngine::AudioClip* _bossVoice3;

private:


	RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
		RTTR_REGISTRATION_FRIEND
};
}