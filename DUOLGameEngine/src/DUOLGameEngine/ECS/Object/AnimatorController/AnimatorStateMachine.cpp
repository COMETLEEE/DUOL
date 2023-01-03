#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"

namespace DUOLGameEngine
{
	AnimatorStateMachine::AnimatorStateMachine(DUOLGameEngine::AnimatorControllerLayer* layer, const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Resource)
		, _animatorStates({})
		, _entryState(nullptr)
		, _animatorControllerLayer(layer)
	{
		// _entryState가 기본적으로 제공이 되면 좋을 것 같은데 .. 그래야 깔끔하게 스테이트 머신에 진입하는 느낌이 들 것 같음 ..
	}

	AnimatorStateMachine::~AnimatorStateMachine()
	{
		for (auto& pair : _animatorStates)
			delete pair.second;

		_animatorStates.clear();
	}

	DUOLGameEngine::AnimatorState* AnimatorStateMachine::AddState(const DUOLCommon::tstring& name)
	{
		DUOLGameEngine::AnimatorState* newState;

		// 중복 이름을 허용하지 않습니다.
		if (_animatorStates.contains(name))
			return AddState(name + TEXT("0"));
		else
			newState = new AnimatorState(this, name);

		// 가장 처음 등록된 State 입니다. => _entryState로 적용해줍니다.
		if (_animatorStates.empty())
		{
			_entryState = newState;

			_animatorStates.insert({ newState->GetName(), newState });
		}
		else
		{
			_animatorStates.insert({ newState->GetName(), newState });
		}

		return newState;
	}

	void AnimatorStateMachine::RemoveState(const DUOLCommon::tstring& name)
	{
		// 만약 해당 이름을 가진 상태가 존재한다면
		if (_animatorStates.contains(name))
		{
			// 메모리에서 해제하고
			delete _animatorStates.at(name);

			// 리스트에서 제거합니다.
			_animatorStates.erase(name);
		}
	}

	void AnimatorStateMachine::RemoveState(DUOLGameEngine::AnimatorState* animatorState)
	{
		for (auto& pair : _animatorStates)
		{
			// 만약 똑같은 AnimatorState가 존재한다면 ..
			if (pair.second == animatorState)
			{
				// 메모리에서 해제하고
				delete animatorState;

				// 리스트에서 제거합니다.
				_animatorStates.erase(pair.first);

				return;
			}
		}
	}

	DUOLGameEngine::AnimatorState* AnimatorStateMachine::GetEntryState() const
	{
		return _entryState;
	}

	void AnimatorStateMachine::UpdateAnimatorStateMachine(DUOLGameEngine::AnimatorControllerContext* context,
		float deltaTime)
	{
		// TODO - 현재 Transition 중인지 체크합니다. (보간 등의 수행 중인 것이니까 ..)
		// TODO - 만약 Transition 중이면 밑의 체크 사항이 아닌 그 기능을 수행하면 됩니다.

		// Context의 현재 스테이트를 가져옵니다.
		DUOLGameEngine::AnimatorState* currentState = context->_currentStates[0];

		// 현재 스테이트의 모든 트랜지션을 체크해서 가능한 Transition을 받습니다.
		DUOLGameEngine::AnimatorStateTransition* transition = currentState->CheckAllTransition(context);

		// 트랜지션 할 수 없는 상황입니다. 현재 스테이트에 대해서 Context를 갱신합니다. 
		if (transition == nullptr)
		{
			NotTransition(context, deltaTime);
		}
		// 트랜지션 할 수 있는 상황입니다. ‘Transition Duration’, ‘Transition Offset’ 에 따라
		// 보간, 변경되도록 구현합니다. TODO - 1월 3일 .. 일단 보간 없다.
		else
		{
			Transition(transition, context);
		}
	}

	void AnimatorStateMachine::NotTransition(DUOLGameEngine::AnimatorControllerContext* targetContext, float deltaTime)
	{
		// TODO - 일단 트랜지션 성능만 확인해보자 ..
		targetContext->_animator->Play(deltaTime, targetContext->_currentStates[0]->GetAnimationClip());
	}

	void AnimatorStateMachine::Transition(DUOLGameEngine::AnimatorStateTransition* targetTransition, DUOLGameEngine::AnimatorControllerContext* context)
	{
		// TODO - 일단 트랜지션 성능만 확인해보자 ..
		context->_currentStates[0] = targetTransition->_to;
	}
}
