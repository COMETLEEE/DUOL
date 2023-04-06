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

#include "DUOLCommon/Event/Event.h"
#include "DUOLGameEngine/ECS/ObjectBase.h"
#include "DUOLMath/DUOLMath.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/AnimationClip.h"

namespace DUOLGraphicsEngine
{
	class AnimationClip;
}

namespace DUOLGameEngine
{
    class GameObject;
}

namespace DUOLGameEngine
{
	/**
	 * \brief AnimationEvent lets you call a function as part of playing back an animation.
	 */
	struct DUOL_GAMEENGINE_API AnimationEvent
    {
	    /**
	     * \brief 해당 이벤트의 이름입니다.
	     */
	    DUOLCommon::tstring                 _eventName = TEXT("");

        /**
         * \brief 해당 이벤트가 터지는 타겟 프레임
         */
        float                               _targetFrame = 0.f;

	    /**
	     * \brief 이벤트 함수에 전달되는 bool 매개변수
	     */
        bool                                _boolParameter = false;

        /**
         * \brief 이벤트 함수에 전달되는 int 매개변수
         */
        int                                 _intParameter = 0;

        /**
         * \brief 이벤트 함수에 전달되는 float 매개변수
         */
        float                               _floatParameter = 0.f;

        /**
         * \brief 이벤트 함수에 전달되는 tstring 매개변수
         */
        DUOLCommon::tstring                 _tstringParameter = TEXT("");
    };

    struct DUOL_GAMEENGINE_API AnimationFrame
    {
    public:
        AnimationFrame()
        {
            _time = 0.f;
            _localTransform = DUOLMath::Vector3::Zero;
            _localRotation = DUOLMath::Quaternion::Identity;
            _localScale = DUOLMath::Vector3::Zero;
        }

        AnimationFrame(DUOLGraphicsEngine::KeyFrame& keyframe)
        {
            memcpy(this, &keyframe, sizeof(AnimationFrame));
        };

        float _time;

        DUOLMath::Vector3	_localTransform;

        DUOLMath::Quaternion _localRotation;

        DUOLMath::Vector3	_localScale;
    };

	class DUOL_GAMEENGINE_API AnimationClip final : public DUOLGameEngine::ObjectBase
	{
	public:
        AnimationClip(const DUOLCommon::tstring& name = TEXT("AnimationClip"));

        virtual ~AnimationClip() override;

	private:
		/**
         * \brief _animationClip's playtime in seconds.
         */
        float _lengthInSeconds;

        /**
         * \brief _animationClip's playtime in frame.
         */
        float _maxFrame;

        float _frameRate;

        float _tickPerFrame;

        int _totalKeyFrame;

        int _startKeyFrame;

        int _endKeyFrame;

        bool _isRootMotion;

        int _rootMotionTargetIndex;                         // 몇 번 인덱스를 기준으로 루트 모션을 일으키나요 ?

        bool _useEventInTransition;

		std::vector<std::vector<AnimationFrame>> _frameList;

    private:
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

        bool GetIsRootMotion() const;

        void SetIsRootMotion(bool value);

        int GetRootMotionTargetIndex() const;

        void SetRootMotionTargetIndex(int value);

        bool GetIsUseEventInTransition() const;

        void SetIsUseEventInTransition(bool value);

	private:
        void GetTargetFrameTransform(int targetFrame, int targetBoneIndex,
            DUOLMath::Vector3& outPos, DUOLMath::Quaternion& outRot, DUOLMath::Vector3& outScale) const;

        void GetTargetFrameTransform(int targetFrame, int targetBoneIndex, DUOLMath::Matrix& outMatrix) const;

        void GetTargetFramesTransform(int fromFrame, int toFrame, int targetBoneIndex, float tFrom, DUOLGameEngine::AnimationClip* toClip, DUOLMath::Matrix& outMatrix) const;

	private:
        /**
         * \brief 매 프레임 갱신마다 해당 애니메이션 클립을 플레이하는 컨텍스트에서 (Transition 포함)
         */
        std::vector<DUOLGameEngine::AnimationEvent> _events;
        
        /**
         * \brief 이전 프레임과 현재 프레임 사이에 등록된 키프레임 이벤트가 있는지 확인하고 있다면 일으킬 준비를 합니다.
         * \param prevFrame 이전 프레임 수치입니다.
         * \param currFrame 현재 프레임 수치입니다.
         */
        bool CheckKeyframeEventAndInvoke(float prevFrame, float currFrame, DUOLGameEngine::GameObject* gameObject);

	public:
        /**
         * \brief DUOLGameEngine::EventManager 에 모든 파라미터에 대해서 이벤트를 추가해줍니다.
         * \param event 해당 애니메이션 클립에 등록할 이벤트입니다.
         */
        void AddEvent(const DUOLGameEngine::AnimationEvent& event);

        RTTR_ENABLE(DUOLGameEngine::ObjectBase)

        RTTR_REGISTRATION_FRIEND
		
#pragma region FRIEND_CLASS
        friend class ResourceManager;

        friend class Animator;
#pragma endregion
	};
}