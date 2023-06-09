#pragma once

#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class Enemy;
	/**
	 * \brief ���� ������ �����ϱ� ���� ����. ��, �޾ƿ��� �𵨸��� �����ϰ� ���õǾ� ����� ������ ���� ����.
	 */
	class DUOL_CLIENT_API BossEnemy_Weapon_Sword : public DUOLGameEngine::MonoBehaviourBase
	{

	public:
		BossEnemy_Weapon_Sword(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Weapon_Sword"));

		virtual ~BossEnemy_Weapon_Sword() override;

	private:
		DUOLClient::Enemy* _owner;

		DUOLGameEngine::Transform* _houseWeapon;

		DUOLGameEngine::Transform* _holdWeapon;

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		void SetOwner(DUOLClient::Enemy* owner);

		void HoldSword();

		void HouseSword();

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase);

	};
}
