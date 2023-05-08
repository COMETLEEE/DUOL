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
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	struct AnimatorControllerContext;

	class AnimatorController;

	class Avatar;

	class AnimationClip;
}

namespace DUOLGameEngine
{
	/**
	 * \brief Interface to control the Mecanim animation system.
	 */
	class DUOL_GAMEENGINE_API Animator : public DUOLGameEngine::BehaviourBase
	{
	public:
		Animator(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Animator"));

		virtual ~Animator() override;

	private:
		/**
		 * \brief 애니메이션의 회전을 담당하는 객체
		 */
		DUOLGameEngine::AnimatorController* _animatorController;

		/**
		 * \brief Controller를 통한 Animation Loop를 위한 컨텍스트.
		 */
		DUOLGameEngine::AnimatorControllerContext* _controllerContext;

		// TODO - 다음에 Blending을 위해서 Avatar (FBX Import 할 때 Rig Info로 나오게 ..) Masking 기능 넣어야함.
#pragma region AVATAR_INFORMATION
		/**
		 * \brief Transform matrix update 할 본들의 배열 (일치하지 않으면 폭발함)
		 */
		std::vector<DUOLGameEngine::GameObject*> _boneGameObjects;

		/**
		 * \brief offset matrix list.
		 */
		std::vector<DUOLMath::Matrix> _boneOffsetMatrixList;

		/**
		 * \brief Shader에 직접 넣을 bone matrix pallet. (_boneOffset..., _boneGameObjects 를 통해 연산한다.)
		 */
		std::vector<DUOLMath::Matrix> _boneMatrixList;
#pragma endregion

		DUOLCommon::EventListenerID _onSceneEditModeUpdatingID;

		float _speed;

	private:
		/**
		 * \brief Calculate Animation Clip's Bone Matrix in current frame. 이게 맞나 싶기는 한데 .. 일단 아바타 블렌드없이 해보자.
		 */
		void Play(float deltaTime, DUOLGameEngine::AnimationClip* animationClip);

		void CalcRootMotion(int prevFrame, int currFrame, DUOLGameEngine::AnimationClip* animationClip);

		void CalcNormalMotion(int currFrame, DUOLGameEngine::AnimationClip* animationClip);

		/**
		 * \brief Calculate Two Animation Clip's Bone matrix in current frame with linear interpolation.
		 * \param deltaTime frame time.
		 * \param fromClip The Motion of state 'from' in transition.
		 * \param toClip The Motion of state 'to' in transition.
		 * \param tFrom The interpolation parameter of from animation clip.
		 */
		void Play(float deltaTime, DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip, float tFrom);

		void CalcRootMotionFrom(int prevFrameRoot, int currFrameFrom, int prevFrameTo, int currFrameTo,
			DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip, float tFrom);

		void CalcRootMotionTo(int prevFromFrame, int currFromFrame, int prevToFrame, int currToFrame,
			DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip, float tFrom);

		void CalcRootMotionFromTo(int prevFromFrame, int currFromFrame, int prevToFrame, int currToFrame,
			DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip, float tFrom);

		void CalcNormalMotion(int prevFromFrame, int currFromFrame, int prevToFrame, int currToFrame,
			DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip, float tFrom);

		/**
		 * \brief 애니메이션은 게임 로직이 업데이트된 후 최종적으로 업데이트합니다.
		 * \param deltaTime deltaTime of current frame.
		 */
		void OnLateUpdate(float deltaTime) override;

	public:
		void SetAnimatorController(DUOLGameEngine::AnimatorController* animatorController);

		DUOLGameEngine::AnimatorController* GetAnimatorController() const;

		void SetBoneGameObjects(const std::vector<DUOLGameEngine::GameObject*>& boneObjects);

		const std::vector<DUOLGameEngine::GameObject*>& GetBoneGameObjects();

		void SetBoneOffsetMatrices(const std::vector<DUOLMath::Matrix>& matrices);

		const std::vector<DUOLMath::Matrix>& GetBoneOffsetMatrices();

		std::vector<DUOLMath::Matrix>* GetBoneMatrices();

		float GetSpeed() const;

		void SetSpeed(float value);

#pragma region ANIMATION_TRANSITION_METHODS
	public:
		/**
		 * \brief Sets the value of the given bool parameter.
		 * \param paramName The parameter name.
		 * \param value The new parameter value.
		 */
		void SetBool(const DUOLCommon::tstring& paramName, bool value) const;

		/**
		 * \brief Sets the value of the given float parameter.
		 * \param paramName The parameter name.
		 * \param value The new parameter value.
		 */
		void SetFloat(const DUOLCommon::tstring& paramName, float value) const;

		/**
		 * \brief Sets the value of the given integer parameter.
		 * \param paramName The parameter name.
		 * \param value The new parameter value.
		 */
		void SetInt(const DUOLCommon::tstring& paramName, int value) const;

		/**
		 * \brief Returns the value of the given bool parameter.
		 * \param paramName The parameter name.
		 * \return The value of the parameter.
		 */
		bool GetBool(const DUOLCommon::tstring& paramName) const;

		/**
		 * \brief Returns the value of the given float parameter.
		 * \param paramName The parameter name.
		 * \return The value of the parameter.
		 */
		float GetFloat(const DUOLCommon::tstring& paramName) const;

		/**
		 * \brief Returns the value of the given int parameter.
		 * \param paramName The parameter name.
		 * \return The value of the parameter. 
		 */
		int GetInt(const DUOLCommon::tstring& paramName) const;

		/**
			@brief Set All Parameter to Zero.
		**/
		void AllParamReset() const;

	public:
		const DUOLCommon::tstring& GetCurrentStateName() const;

#pragma endregion

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		RTTR_REGISTRATION_FRIEND

#pragma region FRIEND_CLASS
		friend class SkinnedMeshRenderer;

		friend class AnimatorController;

		friend class AnimatorControllerLayer;

		friend class AnimatorStateMachine;

		friend class AnimatorState;
#pragma endregion
	};
}