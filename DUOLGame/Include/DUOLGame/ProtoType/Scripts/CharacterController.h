/**

    @file    CharacterController.h
    @brief   
    @details -
    @author  JKim
    @date    20.12.2022

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGame
{
    using namespace DUOLGameEngine;

	class CharacterController : public MonoBehaviourBase
	{
    public:

        CharacterController(const std::weak_ptr<GameObject>& owner);

        ~CharacterController() final;

    public:

        void OnUpdate(float deltaTime) final;
	};
}