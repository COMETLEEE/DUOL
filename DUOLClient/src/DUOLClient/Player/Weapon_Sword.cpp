#include "DUOLClient/Player/Weapon_Sword.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

#include "DUOLClient/Player/Player.h"
#include "DUOLClient/ECS/Component/CharacterBase.h"
#include "DUOLClient/ECS/Component/ParticleData.h"
#include "DUOLClient/Manager/ParticleManager.h"

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

		// �� ������ ����
		GetTransform()->SetLocalScale(DUOLMath::Vector3{ 100.f, 100.f ,100.f });
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
			// TODO : ���� ��� ��
			else if (gameObject->GetTag() == TEXT("Hold_Weapon"))
			{
				_holdWeapon = gameObject->GetTransform();
			}
			// TODO : ����
			else if (gameObject->GetTag() == TEXT("House_Weapon"))
			{
				_houseWeapon = gameObject->GetTransform();
			}
		}

		// �ϴ� ���ۿ� ���� ������.
	}

	void Weapon_Sword::OnUpdate(float deltaTime)
	{
		// TODO : ���� ���� �������� �ʾƼ� �ϴ� ���ο� �༮�� �����մϴ�.
		GetTransform()->SetLocalRotation(DUOLMath::Quaternion::Identity);
	}

	void Weapon_Sword::OnCollisionEnter(const std::shared_ptr<DUOLPhysics::Collision>& collision)
	{
		// ���� ������Ʈ�� �ƴϸ� �Ѿ��.
		if (_player->_playerStateMachine.GetCurrentState()->GetName() != TEXT("PlayerState_Attack"))
			return;

		DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(collision->_other);

		// ���� ������.
		if ((gameObject != nullptr) && (gameObject->GetTag() == TEXT("Enemy")))
		{
			CharacterBase* enemy = gameObject->GetComponent<CharacterBase>();

			if (enemy != nullptr)
			{
				_player->Attack(enemy, _player->_currentDamage, AttackType::LightAttack);

				// TODO : �ǰ� ���� ���

				// TODO : �ǰ� ��ġ�� ����Ʈ ���
				auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 1.0f);
				particleData->GetTransform()->SetPosition(collision->_data[0]._position, DUOLGameEngine::Space::World);

				if (!_player->_isOverdriveSwordMode && !_player->_isOverdriveFistMode)
					_player->_currentOverdrivePoint += OVERDRIVE_POINT_PER_SWORD;
			}
		}
	}

	void Weapon_Sword::HoldSword()
	{
		GetTransform()->SetParent(_holdWeapon, false);
	}

	void Weapon_Sword::HouseSword()
	{
		GetTransform()->SetParent(_houseWeapon, false);
	}
}