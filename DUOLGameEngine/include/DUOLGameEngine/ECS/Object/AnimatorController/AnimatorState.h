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
		 * \brief �ش� ������� ����ϴ� �Ķ������ �̸�
		 */
		DUOLCommon::tstring _parameterName;

		/**
		 * \brief � ��忡�� ������� �����ϴ°� ����
		 */
		AnimatorConditionMode _mode;

		/**
		 * \brief �ش� ������� ������Ű�� ���� �Ѱ�ġ
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
		 * \brief �ش� State�� ��ġ�� StateMachine�� raw pointer.
		 */
		DUOLGameEngine::AnimatorStateMachine* _animatorStateMachine;

		/**
		 * \brief ��� transition���� ���鼭 Parameter�� ���� �����ϴ� transition�� ������, transition�� �����Ѵ�.
		 */
		std::list<DUOLGameEngine::AnimatorStateTransition*> _transitions;

		/**
		 * \brief �ش� AnimatorState ���� ������ AnimationClip Object.
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