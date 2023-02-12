#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorControllerLayer.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"

#include "DUOLGameEngine/ECS/Object/AnimationClip.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::enumeration<DUOLGameEngine::AnimatorConditionMode>("AnimatorConditionMode")
	(
		value("True", DUOLGameEngine::AnimatorConditionMode::True)
		, value("False", DUOLGameEngine::AnimatorConditionMode::False)
		, value("Greater", DUOLGameEngine::AnimatorConditionMode::Greater)
		, value("Less", DUOLGameEngine::AnimatorConditionMode::Less)
		, value("Equals", DUOLGameEngine::AnimatorConditionMode::Equals)
		, value("NotEqual", DUOLGameEngine::AnimatorConditionMode::NotEqual)
	);

	rttr::registration::class_<DUOLGameEngine::AnimatorCondition>("AnimatorCondition")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_parameterName", &DUOLGameEngine::AnimatorCondition::_parameterName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_mode", &DUOLGameEngine::AnimatorCondition::_mode)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_threshold", &DUOLGameEngine::AnimatorCondition::_threshold)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);


	rttr::registration::class_<DUOLGameEngine::AnimatorStateTransition>("AnimatorStateTransition")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_from", &DUOLGameEngine::AnimatorStateTransition::_from)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::UUIDSerializeType::FileUUID)
	)
	.property("_to", &DUOLGameEngine::AnimatorStateTransition::_to)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::UUIDSerializeType::FileUUID)
	)
	.property("_fixedDuration", &DUOLGameEngine::AnimatorStateTransition::_fixedDuration)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_transitionDuration", &DUOLGameEngine::AnimatorStateTransition::_transitionDuration)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_transitionDuration", &DUOLGameEngine::AnimatorStateTransition::_transitionOffset)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_animatorConditions", &DUOLGameEngine::AnimatorStateTransition::_animatorConditions)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);


	rttr::registration::class_<DUOLGameEngine::AnimatorState>("AnimatorState")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_animatorStateMachine", &DUOLGameEngine::AnimatorState::_animatorStateMachine)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::UUIDSerializeType::FileUUID)
	)
	.property("_transitions", &DUOLGameEngine::AnimatorState::_transitions)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_animationClip", &DUOLGameEngine::AnimatorState::_animationClip)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::UUIDSerializeType::Resource)
	);
}

namespace DUOLGameEngine
{
#pragma region ANIMATOR_STATE_TRANSITION
	AnimatorStateTransition::AnimatorStateTransition():
		ObjectBase(TEXT("AnimatorStateTransition"), ObjectType::Resource)
		, _from(nullptr)
		, _to(nullptr)
		, _fixedDuration(false)
		, _transitionDuration(0.4f)
		, _transitionOffset(0.1f)
		, _allParameterTypes(nullptr)
	{

	}

	AnimatorStateTransition::AnimatorStateTransition(DUOLGameEngine::AnimatorState* from,
		DUOLGameEngine::AnimatorState* to, const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Resource)
		, _from(from)
		, _to(to)
		, _fixedDuration(false)
		, _transitionDuration(0.4f)
		, _transitionOffset(0.1f)
	{
		// 해당 애니메이터 스테이트 트랜지션이 속한 Animator에 등록 Parameter Types를 미리 참조해놓습니다.
		_allParameterTypes = &_from->_animatorStateMachine->_animatorController->_allParameterTypes;
	}

	AnimatorStateTransition::~AnimatorStateTransition()
	{
		_animatorConditions.clear();
	}

	bool AnimatorStateTransition::CheckFromIntParameterType(DUOLGameEngine::AnimatorCondition& condition,
		DUOLGameEngine::AnimatorControllerContext* context)
	{
		// Context에서 해당 value를 가지고 있지 않다면 return false
		if (!context->_intParameters.contains(condition._parameterName))
			return false;

		AnimatorConditionMode mode = condition._mode;

		switch (mode)
		{
			case AnimatorConditionMode::Greater :
			{
				return context->_intParameters.at(condition._parameterName) >= static_cast<int>(condition._threshold);
			}

			case AnimatorConditionMode::Less :
			{
				return context->_intParameters.at(condition._parameterName) <= static_cast<int>(condition._threshold);
			}

			case AnimatorConditionMode::Equals:
			{
				return context->_intParameters.at(condition._parameterName) == static_cast<int>(condition._threshold);
			}

			case AnimatorConditionMode::NotEqual:
			{
				return context->_intParameters.at(condition._parameterName) != static_cast<int>(condition._threshold);
			}

			default:
			{
				return false;
			}
		}
	}

	bool AnimatorStateTransition::CheckFromFloatParameterType(DUOLGameEngine::AnimatorCondition& condition,
		DUOLGameEngine::AnimatorControllerContext* context)
	{
		// Context에서 해당 value를 가지고 있지 않다면 return false
		if (!context->_floatParameters.contains(condition._parameterName))
			return false;

		AnimatorConditionMode mode = condition._mode;

		switch (mode)
		{
			case AnimatorConditionMode::Greater:
			{
				return context->_floatParameters.at(condition._parameterName) >= condition._threshold;
			}

			case AnimatorConditionMode::Less:
			{
				return context->_floatParameters.at(condition._parameterName) <= condition._threshold;
			}

			default:
			{
				return false;
			}
		}
	}

	bool AnimatorStateTransition::CheckFromBoolParameterType(DUOLGameEngine::AnimatorCondition& condition,
		DUOLGameEngine::AnimatorControllerContext* context)
	{
		// Context에서 해당 value를 가지고 있지 않다면 return false
		if (!context->_boolParameters.contains(condition._parameterName))
			return false;

		AnimatorConditionMode mode = condition._mode;

		switch (mode)
		{
			case AnimatorConditionMode::True:
			{
				return context->_boolParameters.at(condition._parameterName);
			}

			case AnimatorConditionMode::False:
			{
				return !context->_boolParameters.at(condition._parameterName);
			}

			default:
			{
				return false;
			}
		}
	}

	bool AnimatorStateTransition::CanTransition(DUOLGameEngine::AnimatorControllerContext* context)
	{
		// TODO : 어떻게하면 일반적인 시리얼라이즈로 이 녀석이 잘 들어올 수 있게 해줄까 .. 모든 포인터 타입에 대해서는
		// 정녕 객체 생성할 때 뭔가 ID 도입해야하는건가 ?
		// 올 파라미터 타입이 없다면 들어와주자 ..
		if (_allParameterTypes == nullptr)
			_allParameterTypes = &_from->_animatorStateMachine->_animatorController->_allParameterTypes;

		bool flag = true;

		// Context에 대해서 모든 Condition이 만족하면 true, 아니면 false를 반환합니다.
		for (auto& condition : _animatorConditions)
		{
			// 만약 해당 파라미터가 애니메이터 컨트롤러에 포함되어 있지 않으면 ..
			if (!_allParameterTypes->contains(condition._parameterName))
			{
				// 삭제해주자 ..!
			}
			else
			{
				AnimatorControllerParameterType type = _allParameterTypes->at(condition._parameterName);

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

			// 만약, 한 컨디션이라도 만족시키지 못 한다면 트랜지션할 수 없습니다.
			if (!flag)
				return false;
		}

		// 모든 컨디션에 대해서 통과한다면 트랜지션할 수 있습니다.
		return true;
	}

	DUOLGameEngine::AnimatorCondition* AnimatorStateTransition::AddCondition(const DUOLCommon::tstring& parameterName,
		DUOLGameEngine::AnimatorConditionMode mode, float threshold)
	{
		AnimatorCondition animCon { parameterName, mode, threshold };

		_animatorConditions.push_back(animCon);

		return &animCon;
	}

	void AnimatorStateTransition::RemoveCondition(DUOLGameEngine::AnimatorCondition& condition)
	{
		std::erase_if(_animatorConditions, [&condition](DUOLGameEngine::AnimatorCondition& conditionIn)
			{
				if (conditionIn == condition)
				{
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
			// 트랜지션할 수 있으면 반환합니다.
			// TODO - 나중에 우선순위 같은 기능도 추가되면 좋을 것 같습니다 ..?
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
				// 해제하고
				delete* iter;

				// 리스트에서 제거합니다.
				iter = _transitions.erase(iter);

				return;
			}
			else
				++iter;
		}
	}
#pragma endregion
}