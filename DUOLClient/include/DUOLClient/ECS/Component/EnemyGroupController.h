﻿/**

	@file      EnemyGroupController.h
	@brief     몬스터 한 군집, 무리를 컨트롤 하는 컴포넌트.
	@details   ~
	@author    SinSeongHyeon
	@date      13.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class AI_Enemy;

	class DUOL_CLIENT_API EnemyGroupController : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		EnemyGroupController(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("EnemyGroupController"));

		virtual ~EnemyGroupController() override = default;

	private:
		std::vector<DUOLClient::AI_Enemy*> _Enemys; // 무리에 포함되어 있는 몬스터들.

		float _radius; // 어느 정도 범위에 생성할 것 인가

		int _count; // 얼마나 생성 할 것 인가

		DUOLMath::Vector3 _targetPos; // 어느 위치에 생성 할 것인가.

		bool _isGroupCheck;

	public:
		void Initialize(float radius, float count);

		bool GetIsGroupCheck() { return _isGroupCheck; }

		void SetIsGroupCheck() { _isGroupCheck = true; }

		void CreateEnemy(); // 몬스터 생성.

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}
