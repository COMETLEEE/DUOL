#pragma once
#include "DUOLGameEngine/ECS/ObjectBase.h"

namespace DUOLGameEngine
{
	class AnimationClip;

	class AnimatorState;

	class AnimatorStateMachine;
}

namespace DUOLGameEngine
{
	/**
	 * \brief The mode of the condition.
	 */
	enum class AnimatorConditionMode
	{
		True
		, False
		, Greater
		, Less
		, Equals
		, NotEqual
	};

	/**
	 * \brief Condition that is used to determine if a transition must be taken.
	 */
	class AnimatorCondition
	{
	public:
		/**
		 * \brief 해당 컨디션이 사용하는 파라미터의 이름
		 */
		DUOLCommon::tstring _parameterName;

		/**
		 * \brief 어떤 모드에서 컨디션이 동작하는가 여부
		 */
		AnimatorConditionMode _mode;

		/**
		 * \brief 해당 컨디션을 만족시키는 값의 한계치
		 */
		float _threshold;
	};

	/**
	 * \brief Transition edge in animator controller.
	 */
	class AnimatorStateTransition final : public DUOLGameEngine::ObjectBase
	{
	public:
		AnimatorStateTransition(DUOLGameEngine::AnimatorState* from, DUOLGameEngine::AnimatorState* to,	const DUOLCommon::tstring& name = TEXT("AnimatorStateTransition"));

		virtual ~AnimatorStateTransition() override;

	private:
		DUOLGameEngine::AnimatorState* _from;

		DUOLGameEngine::AnimatorState* _to;

	public:
		std::list<DUOLGameEngine::AnimatorCondition> _animatorCondition;

		/**
		 * \brief Check can transition to 'to' state.
		 * \return true if can transition.
		 */
		bool CanTransition();
	};

	/**
	* \brief State node in animator controller.
	*/
	class AnimatorState final : public DUOLGameEngine::ObjectBase
	{
	public:
		AnimatorState(DUOLGameEngine::AnimatorStateMachine* stateMachine, const DUOLCommon::tstring& name = TEXT("AnimatorState"));

		virtual ~AnimatorState() override;

	private:
		/**
		 * \brief 해당 State가 위치한 StateMachine의 raw pointer.
		 */
		DUOLGameEngine::AnimatorStateMachine* _animatorStateMachine;

		/**
		 * \brief 모든 transition들을 돌면서 Parameter에 따라 만족하는 transition이 있으면, transition을 진행한다.
		 */
		std::list<DUOLGameEngine::AnimatorStateTransition*> _transitions;

		/**
		 * \brief 해당 AnimatorState 에서 실행할 AnimationClip Object.
		 */
		DUOLGameEngine::AnimationClip* _animationClip;

	public:
		/**
		 * \brief Settings animation clip of state.
		 * \param animationClip animation clip to using in this state.
		 */
		void SetAnimationClip(DUOLGameEngine::AnimationClip* animationClip);

		/**
		 * \brief Get animation clip of this state.
		 * \return AnimationClip of this state.
		 */
		DUOLGameEngine::AnimationClip* GetAnimationClip() const;

		/**
		 * \brief Add an outgoing transition to the destination state.
		 * \param destState the destination state.
		 * \return created transition.
		 */
		DUOLGameEngine::AnimatorStateTransition* AddTransition(DUOLGameEngine::AnimatorState* destState);

		/**
		 * \brief Remove a transition from the state.
		 * \param transition Transition to remove.
		 */
		void RemoveTransition(DUOLGameEngine::AnimatorStateTransition* transition);
	};
}