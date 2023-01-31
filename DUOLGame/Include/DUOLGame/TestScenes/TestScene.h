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

        // ��Ȱ�� �׽�Ʈ�� ���ؼ� ����ξ���.
        virtual void Update(float deltaTime) override;
    };
}