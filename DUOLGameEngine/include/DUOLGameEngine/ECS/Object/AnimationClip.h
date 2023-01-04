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
#include "DUOLMath/DUOLMath.h"

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

        /**
         * \brief _animationClip's playtime in seconds.
         */
        float _lengthInSeconds;

        /**
         * \brief _animationClip's playtime in frame.
         */
        float _maxFrame;

    private:
        /**
         * \brief Getter of primitive animation clip.
         * \return The primitive animation clip in graphics module.
         */
        DUOLGraphicsEngine::AnimationClip* GetPrimitiveAnimationClip() const;

        /**
         * \brief Setter of primitive animation clip.
         * \param animationClip The primitive animation clip in graphics module.
         */
        void SetPrimitiveAnimationClip(DUOLGraphicsEngine::AnimationClip* animationClip);

	public:
        /**
         * \brief return clips frame count in one second.
         * \return frame rate at which keyframes are sampled.
         */
        float GetFrameRate() const;

        /**
         * \brief return total playtime of this animation clip.
         * \return Animation length in seconds. 
         */
        float GetTotalPlayTime() const;

        /**
         * \brief return total frame (== max frame) of this animation clip.
         * \return Animation length in frame.
         */
        float GetMaxFrame() const;

	private:
        void GetTargetFrameTransform(int targetFrame, int targetBoneIndex,
            DUOLMath::Vector3& outPos, DUOLMath::Quaternion& outRot, DUOLMath::Vector3& outScale) const;

        void GetTargetFrameTransform(int targetFrame, int targetBoneIndex, DUOLMath::Matrix& outMatrix) const;

        void GetTargetFramesTransform(int fromFrame, int toFrame, int targetBoneIndex, float tFrom, DUOLGameEngine::AnimationClip* toClip, DUOLMath::Matrix& outMatrix) const;

#pragma region ANIM_EVENT
        void AddEvent(const AnimationEvent& event);

#pragma endregion

#pragma region FRIEND_CLASS
        friend class ResourceManager;

        friend class Animator;
#pragma endregion
	};
}