/**

    @file      CometTestScene.h
    @brief     게임 엔진 개발 테스트용 씬
    @details   ~
    @author    COMETLEE
    @date      1.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/

#pragma once
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLGameEngine
{
    class MonoBehaviourBase;
}

namespace DUOLGame
{
	class CometTestScene : public DUOLGameEngine::Scene
	{
	public:
        CometTestScene();

        ~CometTestScene();

	private:
        DUOLGameEngine::MonoBehaviourBase* _moveCon;

        DUOLGameEngine::GameObject* _joyObject;

	public:
        virtual void Awake() override;

        // 원활한 테스트를 위해서 열어두었다.
        virtual void Update(float deltaTime) override;
	};
}