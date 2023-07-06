#pragma once

#include "EnemyHits.h"
#include "EnemyAttacks.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"


namespace DUOLClient
{
	void StopAnimator(DUOLClient::Enemy* enemy)
	{
		enemy->GetAnimator()->SetSpeed(0);
	}

	void SetNavOnRigidbodyOff(DUOLClient::Enemy* enemy)
	{
		enemy->SetNavOnRigidbodyOff();
	}
	void SetNavOffRigidbodyOn(DUOLClient::Enemy* enemy)
	{
		enemy->SetNavOffRigidbodyOn();
	}

	void LerpLookTarget(DUOLClient::Enemy* enemy)
	{
		if (enemy->GetAIController()->GetIsGroupCheck())
			enemy->LerpLookTarget();
	}
	void SuperArmorOff_OnTimer(DUOLClient::Enemy* enemy)
	{
		enemy->GetAIController()->SetSuperArmor(false, enemy->GetAIController()->GetSuperArmorTime());
	}
	void GroggyOff_OnTimer(DUOLClient::Enemy* enemy)
	{
		auto func = [](Enemy* enemy)->DUOLGameEngine::CoroutineHandler
		{
			co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(enemy->GetParameter<float>(TEXT("GroggyTime")));

			enemy->GetAnimator()->SetBool(TEXT("IsGroggy"), false);
		};

		enemy->StartCoroutine_Manual(std::bind(func, enemy));
	}

	void EventSetBool(DUOLClient::Enemy* enemy, DUOLCommon::tstring name, bool isBool)
	{
		enemy->GetAnimator()->SetBool(name, isBool);
	}

	void SetBoolParameter(DUOLClient::Enemy* enemy, DUOLCommon::tstring name, bool isBool)
	{
		enemy->SetParameter(name, isBool);
	}

	void RushParticlePlay(DUOLClient::Enemy* enemy)
	{
		auto particle = ParticleManager::GetInstance()->Pop(ParticleEnum::BigFootRushDustEffect, 1);

		if (!particle) return;

		auto particleTr = particle->GetTransform();

		auto enemyTr = enemy->GetTransform();

		DUOLClient::EnemyManager::GetInstance()->GetMainCameraController()->SetCameraShake(0.4f, DUOLMath::Vector2(3.0f, 3.0f), enemyTr);

		particleTr->SetPosition(enemyTr->GetWorldPosition());

		particleTr->SetRotation(enemyTr->GetWorldRotation());
	}

	void StandingCryParticle(DUOLClient::Enemy* enemy)
	{
		auto particle = ParticleManager::GetInstance()->Pop(ParticleEnum::Shouting, 2.0f);

		auto particleTr = particle->GetTransform();

		auto enemyTr = enemy->GetTransform();

		particleTr->SetPosition(enemyTr->GetWorldPosition() + DUOLMath::Vector3(0, 2.5f, 1.0f));

		particleTr->SetRotation(enemyTr->GetWorldRotation());
	}

	void RandomLookAt(DUOLClient::Enemy* enemy)
	{
		auto tr = enemy->GetParentTransform();

		auto targetPos = enemy->GetTarget()->GetTransform()->GetWorldPosition();

		auto dir = tr->GetWorldPosition() - targetPos;

		dir.y = 0;

		dir.Normalize();

		auto rad = DUOLMath::MathHelper::RandF(-1.57079f, 1.57079f);

		const float _cos = cosf(rad);
		const float _sin = sinf(rad);
		const float x = _cos * dir.x + (-_sin * dir.z);
		const float y = _sin * dir.x + _cos * dir.z;
		dir.x = x;
		dir.z = y;

		tr->LookAt(tr->GetWorldPosition() + dir);
	}

	void HoldSword(DUOLClient::Enemy* enemy)
	{
		auto sword = static_cast<BossEnemy_Weapon_Sword*>(enemy->GetParameter<void*>(TEXT("Sword")));

		sword->HoldSword();
	}

	void HouseSword(DUOLClient::Enemy* enemy)
	{
		auto sword = static_cast<BossEnemy_Weapon_Sword*>(enemy->GetParameter<void*>(TEXT("Sword")));

		sword->HouseSword();
	}

	void DisablingPatternStart(DUOLClient::Enemy* enemy)
	{
		auto funcFogOn = [](Enemy* enemy)->DUOLGameEngine::CoroutineHandler
		{
			auto currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

			float t = 0;

			DUOLMath::Vector3 originColor = DUOLMath::Vector3(0.7f, 0.7f, 0.7f);

			float originDensity = 0.002f;

			// Todo : 무력화 패턴 포그 값 정해지면 설정할 것..
			while (t <= 1.0f)
			{
				t += DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

				float clampT = std::min(t, 1.0f);

				currentScene->SetFogDensity(std::lerp(originDensity, 0.1f, clampT));
				currentScene->SetFogScatteringColor(DUOLMath::Vector3::Lerp(originColor, DUOLMath::Vector3(0.0f, 0.0f, 0.0f), clampT));
				currentScene->UpdateGraphicsSettings();
				co_yield nullptr;
			}
		};

		enemy->StartCoroutine_Manual(std::bind(funcFogOn, enemy));

		auto disablingBoundingSphere = [](Enemy* enemy,
			DUOLCommon::tstring fbxModelName,
			DUOLMath::Vector3 createPos,
			DUOLMath::Vector3 startSize,
			DUOLMath::Vector3 endSize,
			float createSpeed,
			float deleteSpeed,
			float waitTime,
			DUOLGameEngine::Transform* parentTransform)->DUOLGameEngine::CoroutineHandler
		{
			auto currentState = enemy->GetAnimator()->GetCurrentStateName();

			auto funcDestroy = [](DUOLGameEngine::GameObject* gameObject,
				DUOLMath::Vector3 startSize,
				DUOLMath::Vector3 endSize,
				float deleteSpeed)->DUOLGameEngine::CoroutineHandler
			{
				float t = 1.0f;

				gameObject->GetTransform()->SetParent(nullptr);

				while (t >= 0.0f)
				{
					t -= DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime() * deleteSpeed;

					float clampT = std::max(t, 0.0f);

					gameObject->GetTransform()->SetLocalScale(DUOLMath::Vector3::Lerp(startSize, endSize, clampT));
					co_yield nullptr;
				}

				DUOLGameEngine::ObjectBase::Destroy(gameObject);
			};

			auto currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();
			auto gameObject1 = currentScene->CreateFromFBXModel(fbxModelName); // 차 오르는 오브젝트

			auto gameObject2 = currentScene->CreateFromFBXModel(fbxModelName); // 외곽선 오브젝트

			float t = 0;

			createPos = enemy->GetTransform()->GetWorldPosition();

			gameObject1->GetTransform()->SetParent(parentTransform);

			gameObject1->GetTransform()->SetPosition(createPos);

			gameObject2->GetTransform()->SetParent(parentTransform);

			gameObject2->GetTransform()->SetPosition(createPos);

			auto look = enemy->GetTransform()->GetLook();
			look.y = 0;
			look.Normalize();

			gameObject1->GetTransform()->LookAt(createPos + look);

			gameObject2->GetTransform()->LookAt(createPos + look);

			gameObject2->GetTransform()->SetLocalScale(endSize);

			while (t <= 1.0f)
			{
				t += DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime() * createSpeed;

				float clampT = std::min(t, 1.0f);

				gameObject1->GetTransform()->SetLocalScale(DUOLMath::Vector3::Lerp(startSize, endSize, clampT));

				if (currentState != enemy->GetAnimator()->GetCurrentStateName()) break;

				co_yield nullptr;
			}

			co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(waitTime);

			DUOLGameEngine::ObjectBase::Destroy(gameObject2);

			enemy->StartCoroutine_Manual(std::bind(funcDestroy, gameObject1, startSize, endSize, deleteSpeed));
		};

		enemy->StartCoroutine_Manual(std::bind(
			disablingBoundingSphere,
			enemy,
			TEXT("sphere"),	// Todo: 무력화 패턴 메쉬 설정해야함.
			enemy->GetTarget()->GetTransform()->GetWorldPosition(),
			DUOLMath::Vector3(0, 0, 0),
			DUOLMath::Vector3(400.0f, 10.0f, 400.0f),
			0.5f,
			3.0f,
			0.5f,
			enemy->GetTransform()
		));

	}

	void DisablingPatternEnd(DUOLClient::Enemy* enemy)
	{
		auto funcFogOff = [](Enemy* enemy)->DUOLGameEngine::CoroutineHandler
		{

			auto currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

			float t = 1.0f;

			DUOLMath::Vector3 originColor = DUOLMath::Vector3(0.7f, 0.7f, 0.7f);

			float originDensity = 0.002f;

			auto animator = enemy->GetAnimator();

			animator->SetBool(TEXT("IsSwordForm"), animator->GetBool(TEXT("IsFistForm")));

			animator->SetBool(TEXT("IsFistForm"), !animator->GetBool(TEXT("IsFistForm")));

			animator->SetBool(TEXT("IsFormChange"), true);

			enemy->GetAIController()->SetSuperArmor(false, 1.5f);

			while (t >= 0.0f)
			{
				t -= DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

				float clampT = std::max(t, 0.0f);

				// Todo : 무력화 패턴 포그 값 정해지면 설정할 것..

				currentScene->SetFogDensity(std::lerp(originDensity, 0.1f, clampT));
				currentScene->SetFogScatteringColor(DUOLMath::Vector3::Lerp(originColor, DUOLMath::Vector3(0.0f, 0.0f, 0.0f), clampT));
				currentScene->UpdateGraphicsSettings();
				co_yield nullptr;
			}
		};

		enemy->StartCoroutine_Manual(std::bind(funcFogOff, enemy));
	}

	void BossEnemy_GroggyStart(DUOLClient::Enemy* enemy)
	{
		DUOLGameEngine::ParticleRenderer* particle = static_cast<DUOLGameEngine::ParticleRenderer*>(enemy->GetParameter<void*>(TEXT("LeftHandParticle")));

		if (particle)
			particle->Stop();

		particle = static_cast<DUOLGameEngine::ParticleRenderer*>(enemy->GetParameter<void*>(TEXT("RightHandParticle")));

		if (particle)
			particle->Stop();

		auto funcFogOff = [](Enemy* enemy)->DUOLGameEngine::CoroutineHandler
		{
			auto currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

			float t = 1.0f;

			DUOLMath::Vector3 originColor = DUOLMath::Vector3(0.7f, 0.7f, 0.7f);

			float originDensity = 0.002f;

			while (t >= 0.0f)
			{
				t -= DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

				float clampT = std::max(t, 0.0f);

				currentScene->SetFogDensity(std::lerp(originDensity, 0.1f, clampT));
				currentScene->SetFogScatteringColor(DUOLMath::Vector3::Lerp(originColor, DUOLMath::Vector3(0.0f, 0.0f, 0.0f), clampT));
				currentScene->UpdateGraphicsSettings();
				co_yield nullptr;
			}

			t = 0;
			float maxTime = enemy->GetParameter<float>(TEXT("MaxGroggyTime"));

			// 그로기 대기.
			while (t < maxTime)
			{
				t += DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();
				co_yield nullptr;
			}

			// 그로기 끝...!
			auto animator = enemy->GetAnimator();

			animator->SetBool(TEXT("IsSwordForm"), animator->GetBool(TEXT("IsFistForm")));

			animator->SetBool(TEXT("IsFistForm"), !animator->GetBool(TEXT("IsFistForm")));

			animator->SetBool(TEXT("IsFormChange"), true);

			animator->SetBool(TEXT("IsUltimate"), false);

			animator->SetBool(TEXT("IsGroggy"), false);

			enemy->SetParameter(TEXT("IsCanGroggy"), false);

			if (enemy->GetParameter<bool>(TEXT("IsSuperArmor")))
				enemy->GetAIController()->SetSuperArmor(false, 1.5f);

		};


		//auto animator = enemy->GetAnimator();

//animator->SetBool(TEXT("IsSwordForm"), animator->GetBool(TEXT("IsFistForm")));

//animator->SetBool(TEXT("IsFistForm"), !animator->GetBool(TEXT("IsFistForm")));

//animator->SetBool(TEXT("IsFormChange"), true);

//enemy->GetAIController()->SetSuperArmor(false, 1.5f);

		enemy->StartCoroutine_Manual(std::bind(funcFogOff, enemy));


	}

	void PullPalyer(DUOLClient::Enemy* enemy)
	{
		auto tr = enemy->GetTransform();

		auto targetTr = enemy->GetTarget()->GetTransform();

		auto dir = tr->GetWorldPosition() - targetTr->GetWorldPosition();

		constexpr float speed = 1.5f;

		targetTr->SetPosition(targetTr->GetWorldPosition() + dir * DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime() * speed);
	}

	void PlaySound(DUOLClient::Enemy* enemy, EnemyAudioEnum enemyAudioEnum, bool isOverride, unsigned offset = 0)
	{
		offset = DUOLMath::MathHelper::Rand(0, offset);

		enemy->PlaySound(enemyAudioEnum, isOverride, offset);
	}

	void PlaySoundRand(DUOLClient::Enemy* enemy, EnemyAudioEnum a, EnemyAudioEnum b, bool isOverride, unsigned offset = 0)
	{
		offset = DUOLMath::MathHelper::Rand(0, offset);

		int randSelect = DUOLMath::MathHelper::Rand(0, 1);

		if (randSelect == 0)
			enemy->PlaySound(a, isOverride, offset);
		else
			enemy->PlaySound(b, isOverride, offset);
	}

	void PlayVoiceSound(DUOLClient::Enemy* enemy, EnemyAudioEnum enemyAudioEnum, bool isOverride, unsigned offset = 0)
	{
		offset = DUOLMath::MathHelper::Rand(0, offset);

		enemy->PlayVoiceSound(enemyAudioEnum, isOverride, offset);
	}

	void PlayVoiceSoundRand(DUOLClient::Enemy* enemy, EnemyAudioEnum a, EnemyAudioEnum b, bool isOverride, unsigned offset = 0)
	{
		offset = DUOLMath::MathHelper::Rand(0, offset);

		int randSelect = DUOLMath::MathHelper::Rand(0, 1);

		if (randSelect == 0)
			enemy->PlayVoiceSound(a, isOverride, offset);
		else
			enemy->PlayVoiceSound(b, isOverride, offset);
	}

	void Enemy_PlayParticle(DUOLClient::Enemy* enemy, ParticleEnum particleEnum, DUOLMath::Vector3 offset, float time, float lookOffset)
	{
		auto particle = ParticleManager::GetInstance()->Pop(particleEnum, time);

		auto particleTr = particle->GetTransform();

		auto enemyTr = enemy->GetTransform();

		particleTr->SetPosition(enemyTr->GetWorldPosition() + offset + enemyTr->GetLook() * lookOffset);

		particleTr->SetRotation(enemyTr->GetWorldRotation());
	}

	void Enemy_PlayRotateRightParticle(DUOLClient::Enemy* enemy, ParticleEnum particleEnum, DUOLMath::Vector3 offset, float time, float lookOffset, float degree = -90.0f)
	{
		auto particle = ParticleManager::GetInstance()->Pop(particleEnum, time);

		auto particleTr = particle->GetTransform();

		auto enemyTr = enemy->GetTransform();

		DUOLMath::Quaternion rot = DUOLMath::Quaternion::CreateFromAxisAngle(enemyTr->GetRight(), DUOLMath::MathHelper::DegreeToRadian(degree));

		particleTr->SetRotation(rot);

		particleTr->SetPosition(enemyTr->GetWorldPosition() + offset + enemyTr->GetLook() * lookOffset);
	}

	void Enemy_PlayRotateRightParticleSwordEnd(DUOLClient::Enemy* enemy, ParticleEnum particleEnum, DUOLMath::Vector3 offset, float time, float lookOffset, float degree = -90.0f)
	{
		auto particle = ParticleManager::GetInstance()->Pop(particleEnum, time);

		auto particleTr = particle->GetTransform();

		auto enemyTr = enemy->GetTransform();

		DUOLMath::Quaternion rot = DUOLMath::Quaternion::CreateFromAxisAngle(enemyTr->GetRight(), DUOLMath::MathHelper::DegreeToRadian(degree));

		particleTr->SetRotation(rot);

		auto sword = static_cast<BossEnemy_Weapon_Sword*>(enemy->GetParameter<void*>(TEXT("Sword")));

		particleTr->SetPosition(sword->GetTransform()->GetWorldPosition() + offset + enemyTr->GetLook() * lookOffset);
	}

	void Enemy_PlayParticle_Sword(DUOLClient::Enemy* enemy, ParticleEnum particleEnum, DUOLMath::Vector3 offset, float time)
	{
		auto particle = ParticleManager::GetInstance()->Pop(particleEnum, time);

		auto particleTr = particle->GetTransform();

		auto sword = static_cast<BossEnemy_Weapon_Sword*>(enemy->GetParameter<void*>(TEXT("Sword")));

		auto prevParticle = enemy->GetParameter<void*>(TEXT("SwordParticle"));
		if (prevParticle)
		{
			static_cast<DUOLGameEngine::ParticleRenderer*>(prevParticle)->Stop();
		}

		enemy->AddParameter<void*>(TEXT("SwordParticle"), particle);

		enemy->SetParameter<void*>(TEXT("SwordParticle"), particle);

		auto swordTr = sword->GetTransform();

		particleTr->SetParent(swordTr);

		particleTr->SetLocalTM(DUOLMath::Matrix::Identity);

		particleTr->SetLocalEulerAngle(DUOLMath::Vector3(0, 0, 90.0f));

		particleTr->SetLocalPosition(offset);
	}
	void Enemy_PlayParticle_LeftHand(DUOLClient::Enemy* enemy, ParticleEnum particleEnum, DUOLMath::Vector3 offset, float time)
	{
		auto prevParticle = enemy->GetParameter<void*>(TEXT("LeftHandParticle"));
		if (prevParticle)
		{
			static_cast<DUOLGameEngine::ParticleRenderer*>(prevParticle)->Stop();
		}

		auto particle = ParticleManager::GetInstance()->Pop(particleEnum, time);

		auto particleTr = particle->GetTransform();

		auto leftHandTr = static_cast<DUOLGameEngine::Transform*>(enemy->GetParameter<void*>(TEXT("LeftHand")));

		enemy->AddParameter<void*>(TEXT("LeftHandParticle"), particle);

		enemy->SetParameter<void*>(TEXT("LeftHandParticle"), particle);

		particleTr->SetParent(leftHandTr);

		particleTr->SetLocalTM(DUOLMath::Matrix::Identity);

		particleTr->SetLocalScale(DUOLMath::Vector3(100.0f, 100.0f, 100.0f));

		particleTr->SetLocalPosition(offset);
	}
	void Enemy_PlayParticle_RightHand(DUOLClient::Enemy* enemy, ParticleEnum particleEnum, DUOLMath::Vector3 offset, float time)
	{
		auto prevParticle = enemy->GetParameter<void*>(TEXT("RightHandParticle"));
		if (prevParticle)
		{
			static_cast<DUOLGameEngine::ParticleRenderer*>(prevParticle)->Stop();
		}

		auto particle = ParticleManager::GetInstance()->Pop(particleEnum, time);

		auto particleTr = particle->GetTransform();

		auto rightHandTr = static_cast<DUOLGameEngine::Transform*>(enemy->GetParameter<void*>(TEXT("RightHand")));

		enemy->AddParameter<void*>(TEXT("RightHandParticle"), particle);

		enemy->SetParameter<void*>(TEXT("RightHandParticle"), particle);

		particleTr->SetParent(rightHandTr);

		particleTr->SetLocalTM(DUOLMath::Matrix::Identity);

		particleTr->SetLocalScale(DUOLMath::Vector3(100.0f, 100.0f, 100.0f));

		particleTr->SetLocalPosition(offset);
	}


	void Enemy_OffParticle(DUOLClient::Enemy* enemy, DUOLCommon::tstring key)
	{
		DUOLGameEngine::ParticleRenderer* particle = static_cast<DUOLGameEngine::ParticleRenderer*>(enemy->GetParameter<void*>(key));

		if (particle)
			particle->Stop();
	}

}
