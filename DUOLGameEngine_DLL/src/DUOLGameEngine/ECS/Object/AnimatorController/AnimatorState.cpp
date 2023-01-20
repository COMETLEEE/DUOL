#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorControllerLayer.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"

namespace DUOLGameEngine
{
#pragma region ANIMATOR_STATE_TRANSITION
	AnimatorStateTransition::AnimatorStateTransition(DUOLGameEngine::AnimatorState* from,
		DUOLGameEngine::AnimatorState* to, const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Resource)
		, _from(from)
		, _to(to)
		, _fixedDuration(false)
		, _transitionDuration(0.4f)
		, _transitionOffset(0.1f)
	{
		// �ش� �ִϸ����� ������Ʈ Ʈ�������� ���� Animator�� ��� Parameter Types�� �̸� �����س����ϴ�.
		_allParameterTypes = &_from->_animatorStateMachine->_animatorControllerLayer
			->_animatorController->_allParameterTypes;
	}

	AnimatorStateTransition::~AnimatorStateTransition()
	{
		// �޸� ����
		for (auto& condition : _animatorConditions)
			delete condition;

		_animatorConditions.clear();
	}

	bool AnimatorStateTransition::CheckFromIntParameterType(DUOLGameEngine::AnimatorCondition* condition,
		DUOLGameEngine::AnimatorControllerContext* context)
	{
		// Context���� �ش� value�� ������ ���� �ʴٸ� return false
		if (!context->_intParameters.contains(condition->_parameterName))
			return false;

		AnimatorConditionMode mode = condition->_mode;

		switch (mode)
		{
			case AnimatorConditionMode::Greater :
			{
				return context->_intParameters.at(condition->_parameterName) >= static_cast<int>(condition->_threshold);
			}

			case AnimatorConditionMode::Less :
			{
				return context->_intParameters.at(condition->_parameterName) <= static_cast<int>(condition->_threshold);
			}

			case AnimatorConditionMode::Equals:
			{
				return context->_intParameters.at(condition->_parameterName) == static_cast<int>(condition->_threshold);
			}

			case AnimatorConditionMode::NotEqual:
			{
				return context->_intParameters.at(condition->_parameterName) != static_cast<int>(condition->_threshold);
			}

			default:
			{
				return false;
			}
		}
	}

	bool AnimatorStateTransition::CheckFromFloatParameterType(DUOLGameEngine::AnimatorCondition* condition,
		DUOLGameEngine::AnimatorControllerContext* context)
	{
		// Context���� �ش� value�� ������ ���� �ʴٸ� return false
		if (!context->_floatParameters.contains(condition->_parameterName))
			return false;

		AnimatorConditionMode mode = condition->_mode;

		switch (mode)
		{
			case AnimatorConditionMode::Greater:
			{
				return context->_floatParameters.at(condition->_parameterName) >= condition->_threshold;
			}

			case AnimatorConditionMode::Less:
			{
				return context->_floatParameters.at(condition->_parameterName) <= condition->_threshold;
			}

			default:
			{
				return false;
			}
		}
	}

	bool AnimatorStateTransition::CheckFromBoolParameterType(DUOLGameEngine::AnimatorCondition* condition,
		DUOLGameEngine::AnimatorControllerContext* context)
	{
		// Context���� �ش� value�� ������ ���� �ʴٸ� return false
		if (!context->_boolParameters.contains(condition->_parameterName))
			return false;

		AnimatorConditionMode mode = condition->_mode;

		switch (mode)
		{
			case AnimatorConditionMode::True:
			{
				return context->_boolParameters.at(condition->_parameterName);
			}

			case AnimatorConditionMode::False:
			{
				return !context->_boolParameters.at(condition->_parameterName);
			}

			default:
			{
				return false;
			}
		}
	}

	bool AnimatorStateTransition::CanTransition(DUOLGameEngine::AnimatorControllerContext* context)
	{
		bool flag = true;

		// Context�� ���ؼ� ��� Condition�� �����ϸ� true, �ƴϸ� false�� ��ȯ�մϴ�.
		for (auto& condition : _animatorConditions)
		{
			// ���� �ش� �Ķ���Ͱ� �ִϸ����� ��Ʈ�ѷ��� ���ԵǾ� ���� ������ ..
			if (!_allParameterTypes->contains(condition->_parameterName))
			{
				// ���������� ..!
			}
			else
			{
				AnimatorControllerParameterType type = _allParameterTypes->at(condition->_parameterName);

				switch (type)
				{
					case AnimatorControllerParameterType::Bool :
					{
						flag = CheckFromBoolParameterType(condition, context);

						break;
					}

					case AnimatorControllerParameterType::Float :
					{
						flag = CheckFromFloatParameterType(condition, context);

						break;
					}

					case AnimatorControllerParameterType::Int :
					{
						flag = CheckFromIntParameterType(condition, context);

						break;
					}
				}
			}

			// ����, �� ������̶� ������Ű�� �� �Ѵٸ� Ʈ�������� �� �����ϴ�.
			if (!flag)
				return false;
		}

		// ��� ����ǿ� ���ؼ� ����Ѵٸ� Ʈ�������� �� �ֽ��ϴ�.
		return true;
	}

	DUOLGameEngine::AnimatorCondition* AnimatorStateTransition::AddCondition(const DUOLCommon::tstring& parameterName,
		DUOLGameEngine::AnimatorConditionMode mode, float threshold)
	{
		AnimatorCondition* animCon = new AnimatorCondition{ parameterName, mode, threshold };

		_animatorConditions.push_back(animCon);

		return animCon;
	}

	void AnimatorStateTransition::RemoveCondition(DUOLGameEngine::AnimatorCondition* condition)
	{
		std::erase_if(_animatorConditions, [&condition](DUOLGameEngine::AnimatorCondition* conditionIn)
			{
				if (conditionIn == condition)
				{
					// �޸� �������� �ؾ��մϴ� ..!
					delete conditionIn;

					return true;
				}
				else
					return false;
			});
	}

	float AnimatorStateTransition::GetTransitionDuration() const
	{
		return _transitionDuration;
	}

	void AnimatorStateTransition::SetTransitionDuration(float transitionDuration)
	{
		_transitionDuration = transitionDuration;
	}

	float AnimatorStateTransition::GetTransitionOffset() const
	{
		return _transitionOffset;
	}

	void AnimatorStateTransition::SetTransitionOffset(float transitionOffset)
	{
		_transitionOffset = transitionOffset;
	}
#pragma endregion

#pragma region ANIMATOR_STATE
	AnimatorState::AnimatorState(DUOLGameEngine::AnimatorStateMachine* stateMachine, const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Resource)
		, _animatorStateMachine(stateMachine)
		, _transitions({})
		, _animationClip(nullptr)
	{

	}

	AnimatorState::~AnimatorState()
	{
		for (auto& transition : _transitions)
			delete transition;
	}

	DUOLGameEngine::AnimatorStateTransition* AnimatorState::CheckAllTransition(
		DUOLGameEngine::AnimatorControllerContext* context)
	{
		for (auto& transition : _transitions)
		{
			// Ʈ�������� �� ������ ��ȯ�մϴ�.
			// TODO - ���߿� �켱���� ���� ��ɵ� �߰��Ǹ� ���� �� �����ϴ� ..?
			if (transition->CanTransition(context))
				return transition;
		}

		return nullptr;
	}

	void AnimatorState::SetAnimationClip(DUOLGameEngine::AnimationClip* animationClip)
	{
		_animationClip = animationClip;
	}

	DUOLGameEngine::AnimationClip* AnimatorState::GetAnimationClip() const
	{
		return _animationClip;
	}

	DUOLGameEngine::AnimatorStateTransition* AnimatorState::AddTransition(DUOLGameEngine::AnimatorState* destState)
	{
		const auto newTransition = new AnimatorStateTransition(this, destState, 
			this->GetName() + TEXT(" -> ") + destState->GetName());
		 
		_transitions.push_back(newTransition);

		return newTransition;
	}

	void AnimatorState::RemoveTransition(DUOLGameEngine::AnimatorStateTransition* transition)
	{
		for (auto iter = _transitions.begin() ; iter != _transitions.end() ;)
		{
			if (*iter == transition)
			{
				// �����ϰ�
				delete* iter;

				// ����Ʈ���� �����մϴ�.
				iter = _transitions.erase(iter);

				return;
			}
			else
				++iter;
		}
	}
#pragma endregion
}