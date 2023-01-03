#pragma once
#include "DUOLGameEngine/ECS/ObjectBase.h"

namespace DUOLGameEngine
{
	class AnimatorStateTransition;

	struct AnimatorControllerContext;

	class AnimatorControllerLayer;

	class AnimatorState;
}

namespace DUOLGameEngine
{
	/**
	 * \brief represent interaction of states.
	 */
	class AnimatorStateMachine : public DUOLGameEngine::ObjectBase
	{
	public:
		AnimatorStateMachine(DUOLGameEngine::AnimatorControllerLayer* layer, const DUOLCommon::tstring& name = TEXT("AnimatorStateMachine"));

		virtual ~AnimatorStateMachine() override;

	private:
		/**
		 * \brief All states in this state machine.
		 */
		std::unordered_map<DUOLCommon::tstring, DUOLGameEngine::AnimatorState*> _animatorStates;

		/**
		 * \brief Start state of this state machine. registered for the first time.
		 */
		DUOLGameEngine::AnimatorState* _entryState;

		/**
		 * \brief 해당 AnimatorStateMachine 이 위치한 AnimatorControllerLayer
		 */
		DUOLGameEngine::AnimatorControllerLayer* _animatorControllerLayer;

	public:
		DUOLGameEngine::AnimatorState* AddState(const DUOLCommon::tstring& name);

		/**
		 * \brief Remove state has param name.
		 * \param name name of the state to remove.
		 */
		void RemoveState(const DUOLCommon::tstring& name);

		/**
		 * \brief Remove state of param.
		 * \param animatorState The state to remove.
		 */
		void RemoveState(DUOLGameEngine::AnimatorState* animatorState);

		/**
		 * \brief Get entry state of this state machine.
		 * \return The entry state. (registered for the first time to this state machine)
		 */
		DUOLGameEngine::AnimatorState* GetEntryState() const;

		/**
		 * \brief Update context that is in this state machine.
		 * \param context The context for update.
		 * \param deltaTime frame time.
		 */
		void UpdateAnimatorStateMachine(DUOLGameEngine::AnimatorControllerContext* context, float deltaTime);

		/**
		 * \brief Update context when does not transition for this frame.
		 * \param targetContext The context to update.
		 * \param deltaTime frame time.
		 */
		void NotTransition(DUOLGameEngine::AnimatorControllerContext* targetContext, float deltaTime);

		/**
		 * \brief First apply a transition.
		 * \param targetTransition The transition to applying.
		 * \param context The context to update.
		 */
		void Transition(DUOLGameEngine::AnimatorStateTransition* targetTransition, DUOLGameEngine::AnimatorControllerContext* context);

#pragma region FRIEND_CLASS
		friend class AnimatorState;

		friend class AnimatorStateTransition;
#pragma endregion
	};
}