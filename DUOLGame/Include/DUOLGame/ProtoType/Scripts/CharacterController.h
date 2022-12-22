/**

    @file    CharacterController.h
    @brief   
    @details -
    @author  JKim
    @date    20.12.2022

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGame
{
    using namespace DUOLGameEngine;

	class CharacterController : public MonoBehaviourBase
	{
    public:

        CharacterController(const std::weak_ptr<GameObject>& owner);

        ~CharacterController() final;

    private:

        Camera* _camera;

        DUOLMath::Vector2 _prevMousePosition;

    public:

        void OnStart() final;

        void OnLateUpdate(float deltaTime) final;
	};
}