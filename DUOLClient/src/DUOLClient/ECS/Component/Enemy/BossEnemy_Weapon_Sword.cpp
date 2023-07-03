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
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/ECS/Object/Material.h"

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
		DUOLGameEngine::MonoBehaviourBase(owner, name),
		_paperBurnOffset(0)
	{
	}

	BossEnemy_Weapon_Sword::~BossEnemy_Weapon_Sword()
	{
	}

	DUOLGameEngine::CoroutineHandler BossEnemy_Weapon_Sword::HoldSwordCoroutine()
	{
		constexpr float speed = 1.0f;
		while (_paperBurnOffset > 0.0f)
		{
			_paperBurnOffset -= speed * DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();
			_meshRenderer->SetOffset(_paperBurnOffset);
			co_yield nullptr;
		}
	}

	DUOLGameEngine::CoroutineHandler BossEnemy_Weapon_Sword::HouseSwordCoroutine()
	{
		constexpr float speed = 1.0f;
		while (_paperBurnOffset < 2.0f)
		{
			_paperBurnOffset += speed * DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();
			_meshRenderer->SetOffset(_paperBurnOffset);
			co_yield nullptr;
		}
	}

	void BossEnemy_Weapon_Sword::ChangeMeterial(SwordMaterial material)
	{
		if (_currentMaterial == material) return;
		_currentMaterial = material;
		_meshRenderer->DeleteAllMaterial();

		switch (material)
		{
		case SwordMaterial::APPEAR:
		{
			for (auto& iter : _originMaterials)
				_meshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(iter->GetName() + _T("PaperBurn")));

			_meshRenderer->SetPaperBurnColor(DUOLMath::Vector4(0.3f, 1.0f, 0.8f, 1.0f), DUOLMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f));

			_paperBurnOffset = 2;

			_meshRenderer->SetOffset(_paperBurnOffset);

		}
		break;

		case SwordMaterial::DISAPPEAR:
		{
			for (auto& iter : _originMaterials)
				_meshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(iter->GetName() + _T("PaperBurn")));

			_meshRenderer->SetPaperBurnColor(DUOLMath::Vector4(0.3f, 1.0f, 0.8f, 1.0f), DUOLMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f));

			_paperBurnOffset = 0;

			_meshRenderer->SetOffset(_paperBurnOffset);
		}
		break;
		}
	}

	void BossEnemy_Weapon_Sword::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		// 모델 스케일 문제
		GetTransform()->SetLocalScale(DUOLMath::Vector3{ 100.f, 100.f ,100.f });
	}

	void BossEnemy_Weapon_Sword::OnStart()
	{
		MonoBehaviourBase::OnStart();


		// 일단 시작엔 무기 꺼놓자.
	}

	void BossEnemy_Weapon_Sword::OnUpdate(float deltaTime)
	{
		// TODO : 무기 뼈가 존재하지 않아서 일단 새로운 녀석에 부착합니다.
		GetTransform()->SetLocalRotation(DUOLMath::Quaternion::Identity);
	}

	void BossEnemy_Weapon_Sword::SetOwner(DUOLClient::Enemy* owner)
	{
		_owner = owner;

		_currentMaterial = SwordMaterial::NONE;

		if (!_meshRenderer)
		{
			for (auto& iter : GetTransform()->GetChildGameObjects())
			{
				_meshRenderer = iter->GetComponent<DUOLGameEngine::MeshRenderer>();
				if (_meshRenderer) break;
			}

			_originMaterials = _meshRenderer->GetMaterials();
		}

		auto topTr = _owner->GetTransform()->GetParent();

		auto allGOs = topTr->GetAllChildGameObjects();

		_owner->AddParameter(TEXT("Sword"), static_cast<void*>(this));

		// Main cameras transform.
		for (auto gameObject : allGOs)
		{
			// TODO : 검집
			if (gameObject->GetName() == TEXT("spine_02.x"))
			{
				_houseWeapon = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->GetTransform();
				_houseWeapon->SetName(TEXT("HouseWeapon"));
				_houseWeapon->SetLocalPosition(DUOLMath::Vector3(-15.98, -18.15, 36.15));
				_houseWeapon->SetLocalEulerAngle(DUOLMath::Vector3(90.0f, 0.0f, 0.0f));
				_houseWeapon->SetParent(gameObject->GetTransform(), false);
			}
			// TODO : 검을 쥐는 곳
			else if (gameObject->GetName() == TEXT("c_thumb1.r"))
			{
				_holdWeapon = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->GetTransform();
				_holdWeapon->SetName(TEXT("HoldWeapon"));
				_holdWeapon->SetLocalPosition(DUOLMath::Vector3(-3.63, 4.64, 9.37));
				_holdWeapon->SetParent(gameObject->GetTransform(), false);
			}
			else if (gameObject->GetName() == TEXT("c_thumb1.r"))
			{
				_rightHand = gameObject->GetTransform();
				_owner->AddParameter(TEXT("RightHand"), static_cast<void*>(_rightHand));
			}
			else if (gameObject->GetName() == TEXT("c_thumb1.l"))
			{
				_leftHand = gameObject->GetTransform();
				_owner->AddParameter(TEXT("LeftHand"), static_cast<void*>(_leftHand));
			}
		}

		HouseSword();

	}


	void BossEnemy_Weapon_Sword::HoldSword()
	{
		GetTransform()->SetParent(_holdWeapon, false);
		ChangeMeterial(SwordMaterial::APPEAR);

		StartCoroutine(&BossEnemy_Weapon_Sword::HoldSwordCoroutine);
	}

	void BossEnemy_Weapon_Sword::HouseSword()
	{
		GetTransform()->SetParent(_houseWeapon, false);
		ChangeMeterial(SwordMaterial::DISAPPEAR);

		StartCoroutine(&BossEnemy_Weapon_Sword::HouseSwordCoroutine);
	}
}