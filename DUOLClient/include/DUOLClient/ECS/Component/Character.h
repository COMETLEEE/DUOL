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
	class DUOL_CLIENT_API Character : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		Character(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Character")) :
			DUOLGameEngine::MonoBehaviourBase(owner, name), _hp(0), _damage(0), _currentSpeed(0)
		{}

		virtual ~Character() override = default;

	protected:
		float _hp;

		float _damage;

		float _currentSpeed;

	public:
		virtual void Attack(Character* target) abstract;

		virtual void OnHit() abstract;

		virtual void SetHP(float hp) { _hp = hp; }

		virtual float GetHP() { return _hp; }

		virtual void SetDamage(float damage) { _damage = damage; }

		virtual float GetDamage() { return _damage; }

		virtual void SetCurrentSpeed(float currentSpeed) { _currentSpeed = currentSpeed; }

		virtual float GetCurrentSpeed() { return _currentSpeed; }

		bool GetIsDie() { return _hp <= 0; }
	};
}
