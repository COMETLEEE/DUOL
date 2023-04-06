#include "DUOLClient/Player/Weapon_Sword.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLClient/Player/Player.h"
#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"
#include "DUOLClient/ECS/Component/CharacterBase.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::Weapon_Sword>("Weapon_Sword")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLClient
{
	Weapon_Sword::Weapon_Sword(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
	{
	}

	Weapon_Sword::~Weapon_Sword()
	{
	}

	void Weapon_Sword::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		
	}

	void Weapon_Sword::OnStart()
	{
		MonoBehaviourBase::OnStart();

		// �� .. �� ���� ����.
		// Using other game object (e.x. main camera's transform) caching
		auto& allGOs = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		// Main cameras transform.
		for (auto gameObject : allGOs)
		{
			if (gameObject->GetTag() == TEXT("Player"))
			{
				DUOLClient::Player* player = gameObject->GetComponent<DUOLClient::Player>();

				// Main Camera Controller �� ���⿡ �޷��ֽ��ϴ�.
				_player = player;
			}
		}

		// �ϴ� ���ۿ� ���� ������.
	}
	
	void Weapon_Sword::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		// ���� ������Ʈ�� �ƴϸ� �Ѿ��.
		if (_player->_playerStateMachine.GetCurrentState()->GetName() != TEXT("PlayerState_Attack"))
			return;

		MonoBehaviourBase::OnTriggerEnter(trigger);

		DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

		// ���� ������.
		if ((gameObject != nullptr) && (gameObject->GetTag() == TEXT("Enemy")))
		{
			CharacterBase* enemy = gameObject->GetComponent<CharacterBase>();

			if (enemy != nullptr)
			{
				_player->Attack(enemy, _player->_currentDamage);

				// TODO : �ǰ� ���� ���
			}
		}
	}
}