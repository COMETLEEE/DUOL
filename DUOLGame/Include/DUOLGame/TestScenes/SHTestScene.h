/**
/**

    @file      SHTestScene.h
    @brief     파티클 시스템 테스트를 위한 Scene
    @details   ~
    @author    SinSeongHyeon
    @date      27.12.2022
    @copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once

#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

class SHTestScene : public DUOLGameEngine::Scene
{
public:
    SHTestScene();

    ~SHTestScene() = default;

public:
    virtual void Awake() override;
};

