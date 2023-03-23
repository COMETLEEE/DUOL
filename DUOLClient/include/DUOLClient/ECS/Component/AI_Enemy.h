/**

	@file      AI_Enemy.h
	@brief     AI Test위해 간단하게 작성한 샘플 BehaviorTree와 함께 사용하여야 한다.
	@details   ~ BehaviorTree의 초기화를 위해 사용하는 클래스이다...! Controller와 같은 역할.
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
	class Enemy;
	class EnemyGroupController;

	class DUOL_CLIENT_API AI_Enemy : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		AI_Enemy(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("AI_Enemy"));

		virtual ~AI_Enemy() override;
	private:
		// 변수 선언
		DUOLGameEngine::BehaviortreeController* _behaviortreeController; // 캐싱해서 사용.

		EnemyGroupController* _enemyGroupController;

		bool _isToken; // 토큰을 가지고 있는 객체만 공격한다..

		DUOLGameEngine::GameObject* target = nullptr;

		DUOLGameEngine::NavMeshAgent* _navMeshAgent;

		DUOLGameEngine::Animator* _animator;

		Enemy* _enemy; // 내가 조종할 객체, 캐싱
	public:

		void SetAnimConditionReset();

		bool GetIsHitCheck();

		void SetIsHit(bool isHit);

		bool GetIsGroupCheck();

		void SetIsGroupCheck();

		void SetGroupController(EnemyGroupController* enemyGroupController);

		bool GetIsToken() { return _isToken; }

		bool GetIsDie();

		void UseToken();

		DUOLGameEngine::NavMeshAgent* GetNavMeshAgent();

		DUOLGameEngine::Animator* GetAnimator();
	public:
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnStart() override;

		friend class EnemyGroupController;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}
