/**

    @file      AnimationClip.h
    @brief     animation 정보가 담겨져 있는 구조체.
    @details   ~
    @author    COMETLEE
    @date      12.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once

#include <functional>

#include "DUOLGameEngine/ECS/ObjectBase.h"

namespace DUOLGraphicsEngine
{
	class AnimationClip;
}

namespace DUOLGameEngine
{
    struct AnimationEvent
    {
        std::function<void(void)> _function;
    };

	class AnimationClip : public DUOLGameEngine::ObjectBase
	{
	public:
        AnimationClip(const DUOLCommon::tstring& name = TEXT("AnimationClip"));

        virtual ~AnimationClip() override;

	private:
        DUOLGraphicsEngine::AnimationClip* _animationClip;

    private:
        DUOLGraphicsEngine::AnimationClip* GetPrimitiveAnimationClip() const;

        void SetPrimitiveAnimationClip(DUOLGraphicsEngine::AnimationClip* animationClip);

	public:
        /**
         * \brief return clips frame count in one second.
         * \return frame rate at which keyframes are sampled.
         */
        float GetFrameRate() const;

        /**
         * \brief 
         * \return Animation length in seconds. 
         */
        float GetLength() const;

#pragma region ANIM_EVENT
        void AddEvent(const AnimationEvent& event);

#pragma endregion

#pragma region FRIEND_CLASS
        friend class ResourceManager;

        friend class Animator;
#pragma endregion
	};
}