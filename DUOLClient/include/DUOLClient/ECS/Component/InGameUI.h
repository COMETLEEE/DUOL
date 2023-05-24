#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{ 
	/**
	 * \brief 인게임의 UI의 데이터를 게임엔진에 UIManager에 넘겨주고 이를 바탕으로 동작하게 만듭니다.
	 */
	class DUOL_CLIENT_API InGameUI final :public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		InGameUI(DUOLGameEngine::GameObject* owner=nullptr, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("InGameUI"));

		virtual ~InGameUI() override;

		virtual void OnUpdate(float deltaTime) override;

		// UIMode로 바꿔줍니다.
		void UIMode();

		// BulletTimeAllMode로 바꿔줍니다.
		// 다 느려짐
		void BulletTimeAllMode();

		// BulletTimePlayerMode
		// 플레이어빼고 다 느려짐
		void BulletTimePlayerMode();

		// 기본 게임 모드
		void OutInGameUIMode();

		// 클리어하고 가는 것
		void ClearAndGoMain();

		// 다음과 같이 MonoBehaviourBase로의 클래스 계층도가 Reflection 될 수 있도록 지정해줍니다.
		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND

	};

}