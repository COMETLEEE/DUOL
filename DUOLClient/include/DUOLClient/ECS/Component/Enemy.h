/**

	@file      Enemy.h
	@brief     Enemy의 정보, 기능을 가지고 있는 클래스.
	@details   ~
	@author    Cool Guy
	@date      23.03.2023
	@copyright © Cool Guy, 2023. All right reserved.

**/
#pragma once

#include "DUOLClient/ECS/Component/CharacterBase.h"
namespace DUOLClient
{
	class DUOL_CLIENT_API Enemy : public CharacterBase
	{
	public:
		Enemy(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Enemy"));

		virtual ~Enemy() override = default;

	private:
		bool _isHit; // 히트를 당했는가.

		float _attackRange; // 공격 사정거리

		float _targetOffset; // 정렬할 때 타겟과의 거리

		float _lookRange; // 시야거리

	public:
		virtual void OnStart() override;

		virtual void Attack(CharacterBase* target) override;

		virtual void OnHit(CharacterBase* other) override;

		friend class AI_Enemy;

		RTTR_ENABLE(CharacterBase)
			RTTR_REGISTRATION_FRIEND
	};
}
