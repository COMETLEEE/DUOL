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

		// 모델 스케일 문제
		GetTransform()->SetLocalScale(DUOLMath::Vector3{ 100.f, 100.f ,100.f });
	}

	void Weapon_Sword::OnStart()
	{
		MonoBehaviourBase::OnStart();

		// 흠 .. 할 일은 없다.
		// Using other game object (e.x. main camera's transform) caching
		auto& allGOs = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		// Main cameras transform.
		for (auto gameObject : allGOs)
		{
			if (gameObject->GetTag() == TEXT("Player"))
			{
				DUOLClient::Player* player = gameObject->GetComponent<DUOLClient::Player>();

				// Main Camera Controller 는 여기에 달려있습니다.
				_player = player;
			}
			// TODO : 검을 쥐는 곳
			else if (gameObject->GetTag() == TEXT("Hold_Weapon"))
			{
				_holdWeapon = gameObject->GetTransform();
			}
			// TODO : 검집
			else if (gameObject->GetTag() == TEXT("House_Weapon"))
			{
				_houseWeapon = gameObject->GetTransform();
			}
		}

		// 일단 시작엔 무기 꺼놓자.
	}

	void Weapon_Sword::OnUpdate(float deltaTime)
	{
		// TODO : 무기 뼈가 존재하지 않아서 일단 새로운 녀석에 부착합니다.
		GetTransform()->SetLocalRotation(DUOLMath::Quaternion::Identity);
	}

	void Weapon_Sword::OnCollisionEnter(const std::shared_ptr<DUOLPhysics::Collision>& collision)
	{
		// 어택 스테이트가 아니면 넘어가라.
		if (_player->_playerStateMachine.GetCurrentState()->GetName() != TEXT("PlayerState_Attack"))
			return;

		DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(collision->_other);

		// 적만 때린다.
		if ((gameObject != nullptr) && (gameObject->GetTag() == TEXT("Enemy")))
		{
			CharacterBase* enemy = gameObject->GetComponent<CharacterBase>();

			if (enemy != nullptr)
			{
				_player->Attack(enemy, _player->_currentDamage, AttackType::LightAttack);

				// TODO : 피격 사운드 출력

				// TODO : 피격 위치에 이펙트 출력
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