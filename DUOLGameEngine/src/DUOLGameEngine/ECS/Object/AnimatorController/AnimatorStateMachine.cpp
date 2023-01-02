#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

namespace DUOLGameEngine
{
	AnimatorStateMachine::AnimatorStateMachine(const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Resource)
		, _animatorStates({})
	{
		// ù ������ EntryState ���� ����մϴ�.
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

		// �ߺ� �̸��� ������� �ʽ��ϴ�.
		if (_animatorStates.contains(name))
			return AddState(name + TEXT("0"));
		else
			newState = new AnimatorState(this, name);

		_animatorStates.insert({ newState->GetName(), newState });

		return newState;
	}

	void AnimatorStateMachine::RemoveState(const DUOLCommon::tstring& name)
	{
		// ���� �ش� �̸��� ���� ���°� �����Ѵٸ�
		if (_animatorStates.contains(name))
		{
			// �޸𸮿��� �����ϰ�
			delete _animatorStates.at(name);

			// ����Ʈ���� �����մϴ�.
			_animatorStates.erase(name);
		}
	}

	void AnimatorStateMachine::RemoveState(DUOLGameEngine::AnimatorState* animatorState)
	{
		for (auto& pair : _animatorStates)
		{
			// ���� �Ȱ��� AnimatorState�� �����Ѵٸ� ..
			if (pair.second == animatorState)
			{
				_animatorStates.erase(pair.first);

				return;
			}
		}
	}
}