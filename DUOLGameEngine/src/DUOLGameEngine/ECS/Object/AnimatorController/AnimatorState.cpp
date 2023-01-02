#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

namespace DUOLGameEngine
{
#pragma region ANIMATOR_STATE_TRANSITION
	AnimatorStateTransition::AnimatorStateTransition(DUOLGameEngine::AnimatorState* from,
		DUOLGameEngine::AnimatorState* to, const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Resource)
		, _from(from)
		, _to(to)
	{
	}

	AnimatorStateTransition::~AnimatorStateTransition()
	{
		_animatorCondition.clear();
	}

	bool AnimatorStateTransition::CanTransition()
	{
		return true;
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