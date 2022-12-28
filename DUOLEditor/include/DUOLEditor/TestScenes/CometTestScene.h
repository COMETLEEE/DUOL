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

namespace DUOLEditor
{
	class CometTestScene : public DUOLGameEngine::Scene
	{
	public:
        CometTestScene();

        ~CometTestScene();

	public:
        virtual void Awake() override;
	};
}