/**
**/

#pragma once
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLGameEngine
{
    class MonoBehaviourBase;
}

namespace DUOLGame
{
    class TestScene : public DUOLGameEngine::Scene
    {
    public:
        TestScene();

        ~TestScene();

    private:
        DUOLGameEngine::MonoBehaviourBase* _moveCon;

        DUOLGameEngine::GameObject* _joyObject;

    public:
        virtual void Awake() override;

        // 원활한 테스트를 위해서 열어두었다.
        virtual void Update(float deltaTime) override;
    };
}