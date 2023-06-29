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

#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/Manager/CameraEventManager.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SoundManager.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"
#include "DUOLGameEngine/ECS/Component/AudioListener.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"

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
		, _generateTime(2.5f)
	{
	}

	Weapon_Sword::~Weapon_Sword()
	{
	}

	void Weapon_Sword::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		_player = EnemyManager::GetInstance()->GetPlayerCharacterGameObject()->GetComponent<DUOLClient::Player>();

		InitializeSwordMaterial();

		//SFX
		auto soundManager = DUOLGameEngine::SoundManager::GetInstance();

		_swordAudioSource = GetGameObject()->GetComponent<DUOLGameEngine::AudioSource>();
		if(_swordAudioSource == nullptr)
			_swordAudioSource = GetGameObject()->AddComponent<DUOLGameEngine::AudioSource>();
		_swordAttackClip[0] = soundManager->GetAudioClip(TEXT("Sword_FinalAttack_Preset01"));
		_swordAttackClip[1] = soundManager->GetAudioClip(TEXT("Sword_FinalAttack_Preset02"));


		// 흠 .. 할 일은 없다.
		// Using other game object (e.x. main camera's transform) caching
		auto allGOs = EnemyManager::GetInstance()->GetPlayerCharacterGameObject()->GetTransform()->GetAllChildGameObjects();

		// Main cameras transform.
		for (auto gameObject : allGOs)
		{
			// TODO : 검을 쥐는 곳
			if (gameObject->GetTag() == TEXT("Hold_Weapon"))
			{
				_holdWeapon = gameObject->GetTransform();
			}
			// TODO : 검집
			else if (gameObject->GetTag() == TEXT("House_Weapon"))
			{
				_houseWeapon = gameObject->GetTransform();
			}
		}
	}

	void Weapon_Sword::OnStart()
	{
		MonoBehaviourBase::OnStart();

		if (GetTag() == TEXT("Weapon_Sword_Overdrive"))
		{
			GetGameObject()->GetComponent<DUOLGameEngine::BoxCollider>()->SetIsEnabled(false);
		}
		// 일단 시작엔 무기 꺼놓자.
	}

	void Weapon_Sword::OnUpdate(float deltaTime)
	{
		// TODO : 무기 뼈가 존재하지 않아서 일단 새로운 녀석에 부착합니다.
		//GetTransform()->SetLocalRotation(DUOLMath::Quaternion::Identity);

		if (_swordCondition == SwordCondition::Normal)
		{
			GetTransform()->SetLocalRotation(DUOLMath::Quaternion::Identity);
		}
		else if (_swordCondition == SwordCondition::Appear)
		{
			AppearSword(deltaTime);
			GetTransform()->SetLocalRotation(DUOLMath::Quaternion::Identity);
		}
		else if (_swordCondition == SwordCondition::Disappear)
		{
			DisappearSword(deltaTime);
		}
		else
		{
		}
	}

	void Weapon_Sword::OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		CheckAttack(trigger);
	}

	void Weapon_Sword::CheckAttack(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		if (_player == nullptr)
			return;

		// 어택 스테이트가 아니면 넘어가라.
		if (!((_player->_playerStateMachine.GetCurrentState()->GetName() == TEXT("PlayerState_Attack"))
			|| (_player->_playerStateMachine.GetCurrentState()->GetName() == TEXT("PlayerState_Ultimate"))))
		{
			return;
		}

		DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

		// 적만 때린다.
		if ((gameObject != nullptr) && (gameObject->GetTag() == TEXT("Enemy")))
		{
			CharacterBase* enemy = gameObject->GetComponent<CharacterBase>();

			if (enemy != nullptr)
			{
				auto pos = gameObject->GetTransform()->GetWorldPosition();
				auto& enemyuuid = enemy->GetUUID();

				if (!CheckPreviousHitRecord(enemyuuid))
				{
					if (_player->Attack(enemy, _player->_currentDamage, AttackType::LightAttack))
					{
						// TODO : �ǰ� ������ ����
						auto particleRenderer = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 0.7f);
						if (particleRenderer)
						{
							DUOLMath::Vector3 randYOffset = DUOLMath::Vector3(DUOLMath::MathHelper::RandF(0.0f, 0.5f), DUOLMath::MathHelper::RandF(1.0f, 2.0f), DUOLMath::MathHelper::RandF(0.0f, 0.5f));
							particleRenderer->GetTransform()->SetPosition(pos + randYOffset);
						}
						if (_mobHitSoundCount < MAX_SOUND_PLAYER)
						{
							_player->PlaySoundClipInModule(_swordAttackClip[0], _mobHitSoundCount, false);
							//_swordAudioSource->Play();
							_mobHitSoundCount++;
						}

						// TODO : 피격 사운드 출력
						// TODO : 피격 위치에 이펙트 출력
						// auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::MonsterHit, 1.0f);
						// particleData->GetTransform()->SetPosition(collision->_data[0]._position, DUOLGameEngine::Space::World);
						_enemyHitList.push_back(enemyuuid);

						if (!_player->_isOverdriveSwordMode && !_player->_isOverdriveFistMode)
							_player->_currentOverdrivePoint += OVERDRIVE_POINT_PER_SWORD;	
					}
				}
			}
		}
	}

	bool Weapon_Sword::CheckPreviousHitRecord(const DUOLCommon::UUID& uuid)
	{
		if (std::find(_enemyHitList.begin(), _enemyHitList.end(), uuid) != _enemyHitList.end())
		{
			return true;
		}

		return false;
	}

	void Weapon_Sword::ResetAttackList()
	{
		_enemyHitList.clear();
		_playHitSound = true;
		_mobHitSoundCount = 0;
	}

	void Weapon_Sword::OffSword()
	{
		_meshRenderer->SetIsEnabled(false);
	}

	void Weapon_Sword::OnSword()
	{
		_meshRenderer->SetIsEnabled(true);
	}

	void Weapon_Sword::InitializeSwordMaterial()
	{
		//todo:: Sword 오브젝트의 자식은 단일하다는 가정하에 제작.
		auto swordMeshObj = GetTransform()->GetChildren()[0];

		_meshRenderer = swordMeshObj->GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>();
		_meshFilter = swordMeshObj->GetGameObject()->GetComponent<DUOLGameEngine::MeshFilter>();

		// 모델 스케일 문제
		_meshRenderer->GetTransform()->SetLocalScale(DUOLMath::Vector3{ 100.f, 100.f ,100.f });

		auto mats = _meshRenderer->GetMaterials();

		_originMaterials = mats;

		for (auto& iter : mats)
		{
			auto matDesc = iter->GetPrimitiveMaterial()->GetMaterialDesc();
			matDesc._materialName += ("PaperBurn_GenerateSword");
			matDesc._pipelineState = _T("PaperBurn_GenerateSword");

			DUOLCommon::tstring materialName(matDesc._materialName.begin(), matDesc._materialName.end());

			auto genMat = DUOLGameEngine::ResourceManager::GetInstance()->CreateMaterial(materialName, matDesc);
			_appearMaterials.push_back(genMat);

			const auto& noisPath = TEXT("SampleNoise.png");


			//노이즈맵 바인드.
			auto noiseTexture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(noisPath);
			genMat->GetPrimitiveMaterial()->SetTexture(noiseTexture, 4);
		}

		//auto materialCopy = materialDesc;

		//auto paperburnMat = CreateMaterial(materialName + _T("PaperBurn_DownUp"), materialCopy);

		//auto noisPath = defaultPath + TEXT("SampleNoise.png");

		//auto noiseTexture = GetTexture(noisPath);

		//if (!noiseTexture)
		//	noiseTexture = LoadMaterialTexture(defaultPath + TEXT("SampleNoise.png"), noisPath);

		//paperburnMat->SetTexture(noiseTexture, 4);

		//mat = _graphicsEngine->LoadMaterial(primitvieMesh->GetSubMesh(subMeshIndex)->_materialName + _T("PaperBurn_DownUp"));
		//if (mat)
		//{
		//	// GraphicsEngine�� ������ ���͸����� ���� ������ �����ϴ� ����..! 
		//	sMat = std::make_shared<DUOLGameEngine::Material>(primitvieMesh->GetSubMesh(subMeshIndex)->_materialName + _T("PaperBurn_DownUp"));

		//	sMat->SetPrimitiveMaterial(mat);

		//	_materialIDMap.insert({ primitvieMesh->GetSubMesh(subMeshIndex)->_materialName + _T("PaperBurn_DownUp"), sMat });
		//	_resourceUUIDMap.insert({ sMat->GetUUID(), sMat.get() });
		//}

	}

	void Weapon_Sword::AppearSword(float deltaTime)
	{
		auto primitiveMesh = _meshFilter->GetMesh()->GetPrimitiveMesh();

		const float maxHeight = primitiveMesh->_center.y + primitiveMesh->_halfExtents.y;
		const float startHeight = primitiveMesh->_center.y - primitiveMesh->_halfExtents.y;

		const float height = _meshFilter->GetMesh()->GetHeight();

		const float speed = _generateTime * (height);

		float offset = _meshRenderer->GetOffset();

		offset += speed * deltaTime;

		_meshRenderer->SetOffset(offset);

		if (offset >= maxHeight)
		{
			_swordCondition = SwordCondition::Normal;
			SetDefaultMaterial();
		}
	}

	void Weapon_Sword::DisappearSword(float deltaTime)
	{
		auto primitiveMesh = _meshFilter->GetMesh()->GetPrimitiveMesh();

		const float maxHeight = primitiveMesh->_center.y + primitiveMesh->_halfExtents.y;
		const float startHeight = primitiveMesh->_center.y - primitiveMesh->_halfExtents.y;

		const float height = _meshFilter->GetMesh()->GetHeight();

		const float speed = _generateTime * (height);

		float offset = _meshRenderer->GetOffset();

		offset -= speed * deltaTime;

		_meshRenderer->SetOffset(offset);

		if (offset <= startHeight)
		{
			_swordCondition = SwordCondition::None;
			_meshRenderer->SetIsEnabled(false);
			SetDefaultMaterial();
		}
	}

	void Weapon_Sword::SetAppearMaterial()
	{
		_meshRenderer->SetIsEnabled(true);
		_meshRenderer->DeleteAllMaterial();

		for (auto& iter : _appearMaterials)
			_meshRenderer->AddMaterial(iter);

		_meshRenderer->SetPaperBurnColor(DUOLMath::Vector4(0.3f, 1.0f, 0.8f, 1.0f), DUOLMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	void Weapon_Sword::SetDefaultMaterial()
	{
		_meshRenderer->DeleteAllMaterial();

		for (auto& iter : _originMaterials)
			_meshRenderer->AddMaterial(iter);

		_meshRenderer->SetPaperBurnColor(DUOLMath::Vector4(0.3f, 1.0f, 0.8f, 1.0f), DUOLMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		_meshRenderer->SetOffset(0);
	}

	void Weapon_Sword::HoldSword()
	{
		if (_swordCondition != SwordCondition::Appear && _swordCondition != SwordCondition::Normal)
		{
			auto primitiveMesh = _meshFilter->GetMesh()->GetPrimitiveMesh();
			const float startHeight = primitiveMesh->_center.y - primitiveMesh->_halfExtents.y;

			GetTransform()->SetParent(_holdWeapon, false);
			_meshRenderer->GetTransform()->SetLocalScale(DUOLMath::Vector3{ 100.f, 100.f ,100.f });
			GetTransform()->SetLocalPosition(DUOLMath::Vector3{ 0.f, 0.f ,0.f });
			GetTransform()->SetLocalRotation(DUOLMath::Quaternion::Identity);

			_meshRenderer->SetOffset(startHeight);
			_swordCondition = SwordCondition::Appear;
			SetAppearMaterial();
		}
	}

	void Weapon_Sword::HouseSword()
	{
		if (_swordCondition != SwordCondition::Disappear && _swordCondition != SwordCondition::None)
		{
			auto primitiveMesh = _meshFilter->GetMesh()->GetPrimitiveMesh();
			const float maxHeight = primitiveMesh->_center.y + primitiveMesh->_halfExtents.y;

			GetTransform()->SetParent(nullptr, true);
			GetTransform()->SetLocalScale(DUOLMath::Vector3{ 1.f, 1.f ,1.f });
			_meshRenderer->GetTransform()->SetLocalScale(DUOLMath::Vector3{ 1.f, 1.f ,1.f });

			_meshRenderer->SetOffset(maxHeight);
			_swordCondition = SwordCondition::Disappear;
			SetAppearMaterial();
		}

	}
}