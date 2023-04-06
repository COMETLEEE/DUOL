/**

	@file      AI_Enemy.h
	@brief     BehaviorTree와 함께 사용하여야하는 클래스. BehaviorTree에서 다른 클래스에 접근하기 위해서 해당 클래스를 거친다.
	@details   ~ Controller와 같은 역할.
				Character 클래스를 기반으로 조종하는 클래스!
	@author    SinSeongHyeon
	@date      10.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Animator;
	class NavMeshAgent;
	class BehaviortreeController;
}

namespace DUOLClient
{
	enum class EnemyCode;
	class Enemy;
	class EnemyGroupController;

	class DUOL_CLIENT_API AI_EnemyBasic : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		AI_EnemyBasic(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("AI_EnemyBasic"));

		virtual ~AI_EnemyBasic() override;
	private:
		// 변수 선언
		DUOLGameEngine::BehaviortreeController* _behaviortreeController; // 캐싱해서 사용.

		EnemyGroupController* _enemyGroupController;

		Enemy* _enemy; // 내가 조종할 객체, 캐싱

	public:
		void Initialize();

		void SetAnimConditionReset();

		bool GetIsHitCheck();

		void SetIsHit(bool isHit);

		bool GetIsGroupCheck();

		void SetIsGroupCheck();

		void SetGroupController(EnemyGroupController* enemyGroupController);

		void UseToken();

		void TakeToken();

		void SetTarget(DUOLGameEngine::GameObject* target);

		bool GetIsToken() const;

		bool GetIsDie() const;

		DUOLGameEngine::GameObject* GetTarget() const;

		EnemyGroupController* GetGroupController() const;

		DUOLGameEngine::NavMeshAgent* GetNavMeshAgent() const;

		DUOLGameEngine::Animator* GetAnimator() const;

		DUOLGameEngine::BehaviortreeController* GetBehaviorTreeController() const;

	public:
		virtual void OnAwake() override;

		friend class EnemyGroupController;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}
