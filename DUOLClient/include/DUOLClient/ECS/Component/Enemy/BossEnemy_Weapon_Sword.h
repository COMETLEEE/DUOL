#pragma once

#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Material;
	class MeshRenderer;
}

namespace DUOLClient
{
	class Enemy;
	/**
	 * \brief ���� ������ �����ϱ� ���� ����. ��, �޾ƿ��� �𵨸��� �����ϰ� ���õǾ� ����� ������ ���� ����.
	 */
	enum class SwordMaterial
	{
		APPEAR,
		DISAPPEAR,
		NONE
	};

	class DUOL_CLIENT_API BossEnemy_Weapon_Sword : public DUOLGameEngine::MonoBehaviourBase
	{

	public:
		BossEnemy_Weapon_Sword(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Weapon_Sword"));

		virtual ~BossEnemy_Weapon_Sword() override;

	private:
		DUOLClient::Enemy* _owner;

		DUOLGameEngine::Transform* _houseWeapon;

		DUOLGameEngine::Transform* _holdWeapon;

		DUOLGameEngine::MeshRenderer* _meshRenderer;

		float _paperBurnOffset;

		std::vector<DUOLGameEngine::Material*> _originMaterials;

	private:
		DUOLGameEngine::CoroutineHandler HoldSwordCoroutine();

		DUOLGameEngine::CoroutineHandler HouseSwordCoroutine();

		void ChangeMeterial(SwordMaterial material);

		SwordMaterial _currentMaterial;

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
