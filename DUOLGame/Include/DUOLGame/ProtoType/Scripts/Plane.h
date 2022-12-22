/**

    @file    Plane.h
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

	class Plane : public MonoBehaviourBase
	{
    public:

        Plane(const std::weak_ptr<GameObject>& owner);

        ~Plane() final;

    public:

        void OnCollisionEnter(const std::shared_ptr<DUOLPhysics::Collision>& collision) final;
        
        void OnCollisionExit(const std::shared_ptr<DUOLPhysics::Collision>& collision) final;

        void OnStart() final;
	};
}