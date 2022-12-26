/**

    @file    PlayerCharacter.h
    @brief   
    @details -
    @author  JKim
    @date    20.12.2022

**/
#pragma once
#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGame
{
    using namespace DUOLGameEngine;

    class PlayerCharacter
    {
    public:

        PlayerCharacter(GameObject* entity);

        ~PlayerCharacter();

    private:
        GameObject* _entity;

    public:

        void AttachCamera(GameObject* camera);
    };
}