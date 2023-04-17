/**

	@file      EnemyParentObjectObserver.h
	@brief     Enemy Class 의 이벤트 함수를 등록하기 위해 만들어진 클래스
	@details   ~, 추가적으로 EnemyParent와 관련된 기능은 임시적으로 여기서 제작하도록 하자.
	@author    SinSeongHyeon
	@date      17.04.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "Enemy.h"
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class DUOL_CLIENT_API EnemyParentObjectObserver : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		EnemyParentObjectObserver(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("EnemyParentObjectObserver"));

		virtual ~EnemyParentObjectObserver() override = default;
	private:
		DUOLClient::Enemy* _enemyOwner;

	public:
		// virtual void OnCollisionEnter(const std::shared_ptr<DUOLPhysics::Collision>& collision) override;
		// virtual void OnCollisionStay(const std::shared_ptr<DUOLPhysics::Collision>& collision) override;
		// virtual void OnCollisionExit(const std::shared_ptr<DUOLPhysics::Collision>& collision) override;

		friend class DUOLClient::Enemy;
	};
}

