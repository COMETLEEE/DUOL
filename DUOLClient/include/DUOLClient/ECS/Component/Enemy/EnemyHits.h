#pragma once

#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLClient/ECS/Component/Enemy/AI_EnemyBasic.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLClient
{
	inline void NormalEnemyHit(DUOLClient::Enemy* thisEnemy, CharacterBase* other, float damage, AttackType attackType)
	{
		const auto ai = thisEnemy->GetAIController();
		const auto animator = ai->GetAnimator();

		if (!thisEnemy->GetIsDie())
			ai->SetAnimConditionReset();

		animator->SetFloat(TEXT("RandOffset"), DUOLMath::MathHelper::RandF(0, 0.5f));

		thisEnemy->SetIsHit(true);

		thisEnemy->SetHP(thisEnemy->GetHP() - damage);

		ai->SetNavOffRigidbodyOn(); // 바닥에 닿았을 때 다시 켜줘야한다.

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

		switch (attackType)
		{
		case AttackType::HeavyAttack:
		{
			auto dir = thisEnemy->GetTransform()->GetWorldPosition() - other->GetTransform()->GetWorldPosition();
			dir.Normalize();
			const auto height = DUOLMath::MathHelper::RandF(12.0f, 15.0f);
			dir = dir * DUOLMath::MathHelper::RandF(12.0f, 15.0f) + DUOLMath::Vector3(0, height, 0);
			thisEnemy->GetRigidbody()->AddImpulse(dir * 5.0f);

			if (animator->GetSpeed() > 0)
				animator->SetBool(TEXT("IsAirBorne"), true); // 공중 피격 애니메이션과 사망애니메이션이 같다.
		}

		break;
		default:
			break;
		}

	}

	inline void WeakEliteEnemyHit(DUOLClient::Enemy* thisEnemy, CharacterBase* other, float damage, AttackType attackType)
	{
		const auto ai = thisEnemy->GetAIController();
		const auto animator = ai->GetAnimator();

		if (!thisEnemy->GetIsDie())
			ai->SetAnimConditionReset();
		else
			return;

		thisEnemy->SetIsHit(true);

		thisEnemy->SetHP(thisEnemy->GetHP() - damage);

		if (thisEnemy->GetIsSuperArmor())
		{
			thisEnemy->ChangeMaterialOnHit();
			return;
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

		if (thisEnemy->GetIsCanSuperArmor()) // 쿨타임이 다 지났다면,
		{
			thisEnemy->AddSuperArmorGauge(damage);

			// 게이지를 넘으면 슈퍼아머 활성화 시키면서 콤보 어택 실행
			if (thisEnemy->GetCurrentSuperArmorGauge() >= thisEnemy->GetMaxSuperArmorGauge())
			{
				ai->SetAnimConditionReset();
				ai->SetSuperArmor(true, thisEnemy->GetSuperArmorTime());
				animator->SetBool(TEXT("IsComboAttack"), true);
				thisEnemy->ChangeMaterialOnHit();
			}
		}
	}
}

