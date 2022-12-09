/**

    @file      EnableTest.h
    @brief     게임 오브젝트 함수 중 컴포넌트 SetEnable 관련하여 테스트하기 위한 함수
    @details   ~
    @author    COMETLEE
    @date      7.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"

namespace DUOLGame
{
	class EnableTest : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		EnableTest(const std::weak_ptr<DUOLGameEngine::GameObject>& owner);

		virtual ~EnableTest() override;

		DUOLGameEngine::MonoBehaviourBase* _setComponent;

		DUOLGameEngine::CoroutineHandler ChangeEnablePer6Sec();

	public:
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}