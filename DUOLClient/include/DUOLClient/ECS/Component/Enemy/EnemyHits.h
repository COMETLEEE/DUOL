#pragma once

#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLClient
{
	inline bool NormalEnemyHit(DUOLClient::Enemy* thisEnemy, CharacterBase* other, float damage, AttackType attackType)
	{
		if (thisEnemy->GetIsInvincible())
			return false;

		const auto ai = thisEnemy->GetAIController();
		const auto animator = ai->GetAnimator();

		if (animator->GetBool(TEXT("IsAirBorne"))) return false;

		if (!thisEnemy->GetIsDie())
		{
			ai->SetAnimConditionReset();
		}
		animator->SetFloat(TEXT("RandOffset"), DUOLMath::MathHelper::RandF(0, 0.5f));

		thisEnemy->SetParameter(TEXT("IsHit"), true);

		thisEnemy->SetHP(thisEnemy->GetHP() - damage);

		ai->SetNavOffRigidbodyOn(); // �ٴڿ� ����� �� �ٽ� ������Ѵ�.

		if (ai->GetAnimator()->GetSpeed() > 0.0f)
		{
			switch (thisEnemy->GetHitEnum())
			{
			case HitEnum::Front:
				animator->SetBool(TEXT("IsHit_Front"), true);
				thisEnemy->SetHitEnum(HitEnum::Back);
				break;
			case HitEnum::Back:
				animator->SetBool(TEXT("IsHit_Back"), true);
				thisEnemy->SetHitEnum(HitEnum::Front);
				break;
			default:
				break;
			}
		}

		switch (attackType)
		{
		case AttackType::HeavyAttack:
		{
			auto dir = thisEnemy->GetTransform()->GetWorldPosition() - other->GetTransform()->GetWorldPosition();
			dir.Normalize();
			const auto height = DUOLMath::MathHelper::RandF(12.0f, 15.0f);
			dir = dir * DUOLMath::MathHelper::RandF(12.0f, 15.0f) + DUOLMath::Vector3(0, height, 0);

			// ���� ���̴ϱ� �������µ� ? => ������ ���õ� ����
			thisEnemy->GetRigidbody()->AddImpulse(dir * 0.5f);

			if (animator->GetSpeed() > 0)
				animator->SetBool(TEXT("IsAirBorne"), true); // ���� �ǰ� �ִϸ��̼ǰ� ����ִϸ��̼��� ����.
		}

		break;
		default:
			break;
		}
		return true;
	}

	inline bool WeakEliteEnemyHit(DUOLClient::Enemy* thisEnemy, CharacterBase* other, float damage, AttackType attackType)
	{
		const auto ai = thisEnemy->GetAIController();
		const auto animator = ai->GetAnimator();

		if (!thisEnemy->GetIsDie())
			ai->SetAnimConditionReset();
		else
			return false;

		thisEnemy->SetParameter(TEXT("IsHit"), true);

		thisEnemy->SetHP(thisEnemy->GetHP() - damage);

		if (thisEnemy->GetParameter<bool>(TEXT("IsSuperArmor")))
		{
			thisEnemy->ChangeMaterialOnHit();
			return false;
		}

		switch (thisEnemy->GetHitEnum())
		{
		case HitEnum::Front:
			animator->SetBool(TEXT("IsHit_Front"), true);
			thisEnemy->SetHitEnum(HitEnum::Back);
			break;
		case HitEnum::Back:
			animator->SetBool(TEXT("IsHit_Back"), true);
			thisEnemy->SetHitEnum(HitEnum::Front);
			break;
		default:
			break;
		}

		if (thisEnemy->GetParameter<bool>(TEXT("IsCanSuperArmor"))) // ��Ÿ���� �� �����ٸ�,
		{
			thisEnemy->GetAIController()->AddSuperArmorGauge(damage);

			// �������� ������ ���۾Ƹ� Ȱ��ȭ ��Ű�鼭 �޺� ���� ����
			if (thisEnemy->GetAIController()->GetCurrentSuperArmorGauge() >= thisEnemy->GetAIController()->GetMaxSuperArmorGauge())
			{
				ai->SetAnimConditionReset();
				ai->SetSuperArmor(true, thisEnemy->GetAIController()->GetSuperArmorTime());
				animator->SetBool(TEXT("IsComboAttack"), true);
				thisEnemy->ChangeMaterialOnHit();
			}
		}
		return true;
	}

	inline bool EliteEnemyHit(DUOLClient::Enemy* thisEnemy, CharacterBase* other, float damage, AttackType attackType)
	{
		const auto ai = thisEnemy->GetAIController();
		const auto animator = ai->GetAnimator();

		if (thisEnemy->GetIsDie()) return false;

		thisEnemy->SetParameter(TEXT("IsHit"), true);

		thisEnemy->SetHP(thisEnemy->GetHP() - damage);

		if (!thisEnemy->GetParameter<bool>(TEXT("IsSuperArmor")) && !thisEnemy->GetParameter<bool>(TEXT("IsGroggy")))
			ai->SetAnimConditionReset();
		else
		{
			thisEnemy->ChangeMaterialOnHit();
			return true;
		}


		switch (thisEnemy->GetHitEnum())
		{
		case HitEnum::Front:
			animator->SetBool(TEXT("IsHit_Front"), true);
			thisEnemy->SetHitEnum(HitEnum::Back);
			break;
		case HitEnum::Back:
			animator->SetBool(TEXT("IsHit_Back"), true);
			thisEnemy->SetHitEnum(HitEnum::Front);
			break;
		default:
			break;
		}

		if (thisEnemy->GetParameter<bool>(TEXT("IsCanSuperArmor"))) // ��Ÿ���� �� �����ٸ�,
		{
			thisEnemy->GetAIController()->AddSuperArmorGauge(damage);

			// �������� ������ ���۾Ƹ� Ȱ��ȭ ��Ű�鼭 �޺� ���� ����
			if (thisEnemy->GetAIController()->GetCurrentSuperArmorGauge() >= thisEnemy->GetAIController()->GetMaxSuperArmorGauge())
			{
				ai->SetAnimConditionReset();
				ai->SetSuperArmor(true);
				animator->SetBool(TEXT("IsComboAttack"), true);
				thisEnemy->SetParameter(TEXT("IsOnSuperArmorEvent"), true);
				thisEnemy->ChangeMaterialOnHit();
			}
		}
		return true;
	}

	inline bool BossEnemyHit(DUOLClient::Enemy* thisEnemy, CharacterBase* other, float damage, AttackType attackType)
	{
		if (thisEnemy->GetIsInvincible())
			return false;

		const auto ai = thisEnemy->GetAIController();
		const auto animator = ai->GetAnimator();

		if (animator->GetCurrentStateName() == TEXT("Dash")) return false;

		int currentHitCount = thisEnemy->GetParameter<float>(TEXT("CurrentHitCount"));
		int maxHitCount = thisEnemy->GetParameter<float>(TEXT("MaxHitCount"));
		currentHitCount++;

		if (maxHitCount <= currentHitCount)
		{
			animator->SetBool(TEXT("IsDash"), true);
			thisEnemy->SetParameter(TEXT("CurrentHitCount"), 0.0f);
			return false;
		}
		thisEnemy->SetParameter(TEXT("CurrentHitCount"), static_cast<float>(currentHitCount));


		if (!thisEnemy->GetIsDie())
		{
			std::vector<std::pair<DUOLCommon::tstring, bool>> saveConditions;

			saveConditions.push_back({ TEXT("IsFistForm"),animator->GetBool(TEXT("IsFistForm")) });
			saveConditions.push_back({ TEXT("IsSwordForm"),animator->GetBool(TEXT("IsSwordForm")) });
			saveConditions.push_back({ TEXT("IsFormChange"),animator->GetBool(TEXT("IsFormChange")) });

			ai->SetAnimConditionReset();

			for (auto& iter : saveConditions)
				animator->SetBool(iter.first, iter.second);
		}

		thisEnemy->SetParameter(TEXT("IsHit"), true);

		if (!thisEnemy->GetIsDie())
		{
			switch (thisEnemy->GetHitEnum())
			{
			case HitEnum::Front:
				animator->SetBool(TEXT("IsHit_Front"), true);
				thisEnemy->SetHitEnum(HitEnum::Back);
				break;
			case HitEnum::Back:
				animator->SetBool(TEXT("IsHit_Back"), true);
				thisEnemy->SetHitEnum(HitEnum::Front);
				break;
			default:
				break;
			}
		}



		return true;
	}
}

