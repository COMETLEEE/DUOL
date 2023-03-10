/**

	@file      AI_Enemy.h
	@brief     AI Test위해 간단하게 작성한 샘플 BehaviorTree와 함께 사용하여야 한다.
	@details   ~ BehaviorTree의 초기화를 위해 사용하는 클래스이다...!
	@author    SinSeongHyeon
	@date      10.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class BehaviortreeController;
}

namespace DUOLClient
{
	class DUOL_CLIENT_API AI_Enemy : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		AI_Enemy(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("AI_Enemy"));

		virtual ~AI_Enemy() override;
	private:
		// 변수 선언
		DUOLGameEngine::BehaviortreeController* _behaviortreeController; // 캐싱해서 사용.

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}
