#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief 2, 3 뒤쪽인덱스의 머테리얼 데이터를 수정하기위해 만든 컴포넌트
	 */
	class DUOL_CLIENT_API MaterialOptionChanger : public DUOLGameEngine::BehaviourBase
	{
	public:
		MaterialOptionChanger(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("MaterialOptionChanger"));

		virtual ~MaterialOptionChanger() override;

	public:

		/**
	 * \brief 씬이 시작할 때 OnStart 보다 이전에 호출됩니다.
	 * 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
	 */
		virtual void OnAwake() override;

		virtual void OnStart() override;


		void UpdateCurrentInfo();

		int GetMatIdx() const;

		void SetMatIdx(int matIdx);

		int _matIdx;


		DUOLMath::Vector2 _tilingInfo;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}