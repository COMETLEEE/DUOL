#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"

namespace DUOLGameEngine
{
	class BoxCollider;
}

namespace DUOLClient
{
	class Player;
}

namespace DUOLClient
{
	/**
	 * \brief ���� ������ �����ϱ� ���� ����. ��, �޾ƿ��� �𵨸��� �����ϰ� ���õǾ� ����� ������ ���� ����.
	 */
	class Weapon_Sword : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		Weapon_Sword(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Weapon_Sword"));

		virtual ~Weapon_Sword() override;

	private:
		DUOLClient::Player* _player;

		DUOLGameEngine::Transform* _houseWeapon;

		DUOLGameEngine::Transform* _holdWeapon;

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnCollisionEnter(const std::shared_ptr<DUOLPhysics::Collision>& collision) override;

		void HoldSword();

		void HouseWeapon();

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase);
	};
}