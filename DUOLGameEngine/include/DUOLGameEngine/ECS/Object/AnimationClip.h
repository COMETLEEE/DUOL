/**

    @file      AnimationClip.h
    @brief     animation 정보가 담겨져 있는 구조체.
    @details   ~
    @author    COMETLEE
    @date      12.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once

#include "DUOLGameEngine/ECS/ObjectBase.h"

namespace DUOLGameEngine
{
	class AnimationClip : public DUOLGameEngine::ObjectBase
	{
	public:
        AnimationClip(const DUOLCommon::tstring& name = TEXT("AnimationClip"));

        virtual ~AnimationClip() override;

	public:
        // 위치

        // 회전

        // 스케일
	};
}