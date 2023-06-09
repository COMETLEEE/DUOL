#include "DUOLClient/ECS/Component/Enemy/BossEnemy_Weapon_Sword.h"

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

#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::BossEnemy_Weapon_Sword>("BossEnemy_Weapon_Sword")
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
	BossEnemy_Weapon_Sword::BossEnemy_Weapon_Sword(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
	{
	}

	BossEnemy_Weapon_Sword::~BossEnemy_Weapon_Sword()
	{
	}

	void BossEnemy_Weapon_Sword::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		// �� ������ ����
		GetTransform()->SetLocalScale(DUOLMath::Vector3{ 100.f, 100.f ,100.f });
	}

	void BossEnemy_Weapon_Sword::OnStart()
	{
		MonoBehaviourBase::OnStart();


		// �ϴ� ���ۿ� ���� ������.
	}

	void BossEnemy_Weapon_Sword::OnUpdate(float deltaTime)
	{
		// TODO : ���� ���� �������� �ʾƼ� �ϴ� ���ο� �༮�� �����մϴ�.
		GetTransform()->SetLocalRotation(DUOLMath::Quaternion::Identity);
	}

	void BossEnemy_Weapon_Sword::SetOwner(DUOLClient::Enemy* owner)
	{
		_owner = owner;

		auto topTr = _owner->GetTransform()->GetParent();

		auto allGOs = topTr->GetAllChildGameObjects();

		// Main cameras transform.
		for (auto gameObject : allGOs)
		{
			// TODO : ����
			if (gameObject->GetName() == TEXT("spine_02.x"))
			{
				_houseWeapon = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->GetTransform();
				_houseWeapon->SetName(TEXT("HouseWeapon"));
				_houseWeapon->SetLocalPosition(DUOLMath::Vector3(-15.98, -18.15, 36.15));
				_houseWeapon->SetLocalEulerAngle(DUOLMath::Vector3(90.0f, 0.0f, 0.0f));
				_houseWeapon->SetParent(gameObject->GetTransform(), false);
			}
			// TODO : ���� ��� ��
			else if (gameObject->GetName() == TEXT("c_thumb1.r"))
			{
				_holdWeapon = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->GetTransform();
				_holdWeapon->SetName(TEXT("HoldWeapon"));
				_holdWeapon->SetLocalPosition(DUOLMath::Vector3(-3.63, 4.64, 9.37));
				_holdWeapon->SetParent(gameObject->GetTransform(), false);
			}
		}

		HouseSword();

	}


	void BossEnemy_Weapon_Sword::HoldSword()
	{
		GetTransform()->SetParent(_holdWeapon, false);
	}

	void BossEnemy_Weapon_Sword::HouseSword()
	{
		GetTransform()->SetParent(_houseWeapon, false);
	}
}