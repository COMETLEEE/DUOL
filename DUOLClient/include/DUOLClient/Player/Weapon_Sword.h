#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"

namespace DUOLGameEngine
{
	class Material;
	class MeshFilter;
	class MeshRenderer;
	class BoxCollider;
}

namespace DUOLClient
{
	class Player;
}

enum class SwordCondition
{
	Normal,
	Appear,
	Disappear,
	None
};

namespace DUOLClient
{
	/**
	 * \brief 무기 변경을 대응하기 위해 만듦. 또, 받아오는 모델링의 스케일과 관련되어 생기는 문제를 막기 위함.
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

		DUOLGameEngine::MeshRenderer* _meshRenderer;

		DUOLGameEngine::MeshFilter* _meshFilter;

		std::vector<DUOLGameEngine::Material*> _originMaterials;

		std::vector<DUOLGameEngine::Material*> _appearMaterials;

		SwordCondition _swordCondition;

		float _generateTime;

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		void OffSword();

		void OnSword();

		void InitializeSwordMaterial();

		void AppearSword(float deltaTime);

		void DisappearSword(float deltaTime);

		void SetAppearMaterial();

		void SetDefaultMaterial();

		void HoldSword();

		void HouseSword();

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase);
	};
}