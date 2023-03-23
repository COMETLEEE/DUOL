/**

	@file      Character.h
	@brief     캐릭터 추상클래스. -> 상속 받은 플레이어나 몬스터를 제작할 것.
	@details   ~
	@author    ShinSeonghyeon
	@date      23.03.2023
	@copyright © Cool Guy, 2023. All right reserved.

**/

#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class DUOL_CLIENT_API CharacterBase : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		CharacterBase(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("CharacterBase"));

		virtual ~CharacterBase() override = default;

	protected:
		float _hp;

		float _damage;

		float _currentSpeed;

	public:
		// 내가 공격 하는 함수.
		virtual void Attack(CharacterBase* other /* 내가 공격할 대상 */) abstract;

		// 내가 공격 당하는 함수.
		virtual void OnHit(CharacterBase* other /* 나를 공격한 대상 */) abstract;

		virtual void SetHP(float hp) { _hp = hp; }

		virtual float GetHP() { return _hp; }

		virtual void SetDamage(float damage) { _damage = damage; }

		virtual float GetDamage() { return _damage; }

		virtual void SetCurrentSpeed(float currentSpeed) { _currentSpeed = currentSpeed; }

		virtual float GetCurrentSpeed() { return _currentSpeed; }

		bool GetIsDie() { return _hp <= 0; }

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}
