#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"

namespace DUOLGameEngine
{
	class BoxCollider;
}

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

	public:
		virtual void OnStart() override;
		
		virtual void OnUpdate(float deltaTime) override;

		virtual void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		virtual void OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		virtual void OnTriggerExit(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase);
	};
}