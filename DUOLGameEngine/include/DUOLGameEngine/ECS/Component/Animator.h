/**

    @file      Animator.h
    @brief     Animation system component.
    @details   ~
    @author    COMETLEE
    @date      12.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	class Avatar;

	class AnimationClip;
}

namespace DUOLGameEngine
{
	/**
	 * \brief Interface to control the Mecanim animation system.
	 */
	class Animator : public DUOLGameEngine::BehaviourBase
	{
	public:
		Animator(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = TEXT("Animator"));

		virtual ~Animator() override;

		/**
		 * \brief TODO : 현재의 애니메이션 클립이 있을 것이 아니라 Animation Controller 가 들어가서 트랜지션, 블렌딩 등을 담당해주
		 */
		DUOLGameEngine::AnimationClip* _currentAnimationClip;

		/**
		 * \brief 사용하는 뼈대
		 */
		DUOLGameEngine::Avatar* _currentAvatar;

	private:


	private:
		/**
		 * \brief Calc Animation Clip's Bone Matrix in current frame.
		 * =>
		 */
		void ComputeAnimationTM();

#pragma region FRIEND_CLASS
		friend class SkinnedMeshRenderer;
#pragma endregion
	};
}