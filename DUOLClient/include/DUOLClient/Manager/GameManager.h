#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief 게임 매니저. 씬 이동에 따른 플레이어의 현재 상태 저장, 시간, 환경설정 등을 담당한다.
	 */
	class GameManager : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		GameManager(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("GameManager"));

		virtual ~GameManager() override;
		
	private:
		float	_testTime;

		bool	_isFirst;

		DUOLCommon::tstring _firstSceneName;

		DUOLCommon::tstring _secondSceneName;

	public:
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}