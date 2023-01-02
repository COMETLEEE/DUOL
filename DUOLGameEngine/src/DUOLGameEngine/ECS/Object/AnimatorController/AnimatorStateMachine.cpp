#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

namespace DUOLGameEngine
{
	AnimatorStateMachine::AnimatorStateMachine(const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Resource)
		, _animatorStates({})
	{
		// 첫 시작은 EntryState 에서 출발합니다.
		_entryState = new AnimatorState(this, TEXT("EntryState"));

		_animatorStates.insert({ _entryState->GetName(), _entryState });
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

		_animatorStates.insert({ newState->GetName(), newState });

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
				_animatorStates.erase(pair.first);

				return;
			}
		}
	}
}