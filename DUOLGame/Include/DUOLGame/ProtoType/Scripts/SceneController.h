#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

#include "DUOLGame/ProtoType/Objects/PlayerCharacter.h"
#include "DUOLGame/ProtoType/Scripts/CameraController.h"

namespace DUOLGame
{
    using namespace DUOLGameEngine;

    class SceneController : public MonoBehaviourBase
    {
    public:

        SceneController(const std::weak_ptr<GameObject>& owner);

        ~SceneController();

    private:
        PlayerCharacter* _player;

        bool _isPlayerControl;

    public:

        void OnUpdate(float deltaTime) final;

        void SetPlayer(PlayerCharacter* player);
    };
}