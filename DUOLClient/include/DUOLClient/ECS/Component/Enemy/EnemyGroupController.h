/**

	@file      EnemyGroupController.h
	@brief     몬스터 한 군집, 무리를 컨트롤 하는 컴포넌트.
	@details   ~ 이름으로 탐색하니 겹치치 않도록 하자...!
	@author    SinSeongHyeon
	@date      13.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class Enemy;
	class AI_EnemyBasic;
	struct DUOL_CLIENT_API EnemyCreateInfo
	{
		EnemyCreateInfo() :
			_closeEnemyCount(0),
			_farEnemyCount(0),
			_weakEliteEnemyCount(0),
			_eliteEnemyCount(0),
			_bossEnemyCount(0),
			_createWaitForSeconds(0.00f),
			_createPos()
		{}

		DEFINE_DEFAULT_COPY_MOVE(EnemyCreateInfo)

			int _closeEnemyCount;

		int _farEnemyCount;

		int _weakEliteEnemyCount;

		int _eliteEnemyCount;

		int _bossEnemyCount;

		float _createWaitForSeconds;

		DUOLMath::Vector3 _createPos;

		RTTR_REGISTRATION_FRIEND
			RTTR_ENABLE()
	};

	class DUOL_CLIENT_API EnemyGroupController : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		EnemyGroupController(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("EnemyGroupController"));

		virtual ~EnemyGroupController() override = default;

	private:
		std::unordered_map<DUOLCommon::UUID, DUOLClient::AI_EnemyBasic*> _enemys; // 무리에 포함되어 있는 몬스터들.

		float _radius; // 어느 정도 범위에 생성할 것 인가

		bool _isGroupCheck; // 그룹에 속한 유닛 중에 적을 발견한 유닛이 있는가?

		bool _isAlignment; // 유닛이 플레이어 기준으로 정렬 하였는가?

		bool _testinit = false;

		int _tokkenCount;

		float _cohesion;

		float _alignment;

		float _separation;

		std::vector<std::pair<EnemyCreateInfo, EnemyCreateInfo>> _firstAndSecondInfo;

		DUOLMath::Vector3 _enemyGroupCenterPos;

		bool _isOnceGroupCenter;
	private:
		DUOLClient::Enemy* PopEnemy(DUOLCommon::tstring name, const DUOLMath::Vector3& targetPos);

		DUOLGameEngine::CoroutineHandler CreateEnemyCoroutine(); // 몬스터 생성.

	public:
		const std::unordered_map<DUOLCommon::UUID, DUOLClient::AI_EnemyBasic*>& GetGroupEnemys();

		float GetCohesion() { return _cohesion; }

		float GetAlignment() { return _alignment; }

		float GetSeparation() { return _separation; }

		bool GetIsGroupCheck();

		void SetIsGroupCheck() { _isGroupCheck = true; }

		void SetCreateInfo(const EnemyCreateInfo& firstInfo, const EnemyCreateInfo& secondInfo);

		void CreateEnemy();

		void RetureTokken() { _tokkenCount++; }

		void EraseEnemy(DUOLCommon::UUID uuid);

		DUOLMath::Vector3 GetGroupCenterPos();

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnLateUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}