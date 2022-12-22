/**

    @file    ProtoType.h
    @brief   
    @details -
    @author  JKim
    @date    20.12.2022

**/
#pragma once
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLGame
{
    using namespace DUOLGameEngine;

	class ProtoTypeScene : public Scene
	{
    public:

        ProtoTypeScene();

        ~ProtoTypeScene();

    public:

        void Awake() final;
	};
}