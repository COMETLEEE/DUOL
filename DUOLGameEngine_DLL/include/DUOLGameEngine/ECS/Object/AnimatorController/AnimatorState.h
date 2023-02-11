#pragma once
#include "DUOLGameEngine/ECS/ObjectBase.h"

namespace DUOLGameEngine
{
	enum class AnimatorControllerParameterType;
	
	class AnimationClip;

	class AnimatorState;

	class AnimatorStateMachine;

	struct AnimatorControllerContext;
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
	class DUOL_GAMEENGINE_API AnimatorCondition
	{
	public:
		bool operator ==(AnimatorCondition& condition)
		{
			return _parameterName == condition._parameterName && _mode == condition._mode && _threshold == condition._threshold;
		}

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
	class DUOL_GAMEENGINE_API AnimatorStateTransition final : public DUOLGameEngine::ObjectBase
	{
	public:
		AnimatorStateTransition();

		AnimatorStateTransition(DUOLGameEngine::AnimatorState* from, DUOLGameEngine::AnimatorState* to,	const DUOLCommon::tstring& name = TEXT("AnimatorStateTransition"));

		virtual ~AnimatorStateTransition() override;

	private:
		/**
		 * \brief Start point of transition.
		 */
		DUOLGameEngine::AnimatorState* _from;

		/**
		 * \brief End point of transition.
		 */
		DUOLGameEngine::AnimatorState* _to;

		/**
		 * \brief true이면 트랜지션의 지속 시간을 초로 나타냅니다. false이면 From 상태 총 시간의 비율 시간으로 나타냅니다.
		 */
		bool _fixedDuration;

		/**
		 * \brief 현재 상태의 지속 시간을 기준으로 상대적인 트랜지션의 지속 시간입니다. 
		 */
		float _transitionDuration;

		/**
		 * \brief 전환될 To 상태에서의 시작할 시간의 퍼센트 오프셋입니다. 예를 들어, 값이 0.5일 경우 To 상태의
		 * 50% 지점에서 플레이를 시작합니다.
		 */
		float _transitionOffset;

	private:
		/**
		 * \brief All conditions of this transition.
		 */
		std::vector<DUOLGameEngine::AnimatorCondition> _animatorConditions;

		/**
		 * \brief AnimatorController 의 Parameter List를 참조해놓습니다.
		 */
		const std::unordered_map<DUOLCommon::tstring, DUOLGameEngine::AnimatorControllerParameterType>* _allParameterTypes;
		
	private:
		bool CheckFromIntParameterType(DUOLGameEngine::AnimatorCondition& condition, DUOLGameEngine::AnimatorControllerContext* context);

		bool CheckFromFloatParameterType(DUOLGameEngine::AnimatorCondition& condition, DUOLGameEngine::AnimatorControllerContext* context);

		bool CheckFromBoolParameterType(DUOLGameEngine::AnimatorCondition& condition, DUOLGameEngine::AnimatorControllerContext* context);

	public:
		/**
		 * \brief Check can transition to 'to' state.
		 * \return true if can transition.
		 */
		bool CanTransition(DUOLGameEngine::AnimatorControllerContext* context);

		DUOLGameEngine::AnimatorCondition* AddCondition(const DUOLCommon::tstring& parameterName, DUOLGameEngine::AnimatorConditionMode mode, float threshold = 0.f);

		void RemoveCondition(DUOLGameEngine::AnimatorCondition& condition);

		float GetTransitionDuration() const;

		void SetTransitionDuration(float transitionDuration);

		float GetTransitionOffset() const;

		void SetTransitionOffset(float transitionOffset);

		RTTR_ENABLE(ObjectBase)

		RTTR_REGISTRATION_FRIEND

#pragma region FRIEND_CLASS
		friend class AnimatorController;

		friend class AnimatorControllerLayer;

		friend class AnimatorStateMachine;

		friend class AnimatorState;
#pragma endregion
	};

	/**
	* \brief State node in animator controller.
	*/
	class DUOL_GAMEENGINE_API AnimatorState final : public DUOLGameEngine::ObjectBase
	{
	public:
		AnimatorState(DUOLGameEngine::AnimatorStateMachine* stateMachine = nullptr, const DUOLCommon::tstring& name = TEXT("AnimatorState"));

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

	private:
		/**
		 * \brief 모든 트랜지션에 대해서 체크합니다. 가장 먼저 확인되는 트랜지션의 주소를 반환합니다. 
		 * \param context 확인하기 위한 Context.
		 * \return 스테이트 트랜지션이 가능한 트랜지션의 주소를 반환합니다. 만약 없다면 nullptr 을 반환합니다.
		 */
		DUOLGameEngine::AnimatorStateTransition* CheckAllTransition(DUOLGameEngine::AnimatorControllerContext* context);

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

		RTTR_ENABLE(DUOLGameEngine::ObjectBase)

		RTTR_REGISTRATION_FRIEND

#pragma region FRIEND_CLASS
		friend class AnimatorStateTransition;

		friend class AnimatorStateMachine;

		friend class AnimatorState;
#pragma endregion
	};
}