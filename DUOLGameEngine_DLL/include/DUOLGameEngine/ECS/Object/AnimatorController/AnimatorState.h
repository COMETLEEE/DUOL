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
		 * \brief true�̸� Ʈ�������� ���� �ð��� �ʷ� ��Ÿ���ϴ�. false�̸� From ���� �� �ð��� ���� �ð����� ��Ÿ���ϴ�.
		 */
		bool _fixedDuration;

		/**
		 * \brief ���� ������ ���� �ð��� �������� ������� Ʈ�������� ���� �ð��Դϴ�. 
		 */
		float _transitionDuration;

		/**
		 * \brief ��ȯ�� To ���¿����� ������ �ð��� �ۼ�Ʈ �������Դϴ�. ���� ���, ���� 0.5�� ��� To ������
		 * 50% �������� �÷��̸� �����մϴ�.
		 */
		float _transitionOffset;

	private:
		/**
		 * \brief All conditions of this transition.
		 */
		std::vector<DUOLGameEngine::AnimatorCondition> _animatorConditions;

		/**
		 * \brief AnimatorController �� Parameter List�� �����س����ϴ�.
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

	private:
		/**
		 * \brief ��� Ʈ�����ǿ� ���ؼ� üũ�մϴ�. ���� ���� Ȯ�εǴ� Ʈ�������� �ּҸ� ��ȯ�մϴ�. 
		 * \param context Ȯ���ϱ� ���� Context.
		 * \return ������Ʈ Ʈ�������� ������ Ʈ�������� �ּҸ� ��ȯ�մϴ�. ���� ���ٸ� nullptr �� ��ȯ�մϴ�.
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