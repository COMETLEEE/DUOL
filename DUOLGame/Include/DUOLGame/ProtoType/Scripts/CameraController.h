/**

    @file    CameraController.h
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

	class CameraController : public MonoBehaviourBase
	{
    public:

        CameraController(const std::weak_ptr<GameObject>& owner);

        ~CameraController() final;

    private:

        GameObject* _cameraController;

        DUOLMath::Vector2 _prevMousePosition;

        std::function<void(Transform*, DUOLMath::Vector2&)> _cameraUpdate;

        bool _isAttached;

    public:

        void OnStart() final;

        void OnLateUpdate(float deltaTime) final;

        GameObject* GetCameraController() const;

        void SwitchingCameraMode();
	};
}