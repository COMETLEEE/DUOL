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
		// _entryState�� �⺻������ ������ �Ǹ� ���� �� ������ .. �׷��� ����ϰ� ������Ʈ �ӽſ� �����ϴ� ������ �� �� ���� ..
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

		// ���� ó�� ��ϵ� State �Դϴ�. => _entryState�� �������ݴϴ�.
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
				// �޸𸮿��� �����ϰ�
				delete animatorState;

				// ����Ʈ���� �����մϴ�.
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
		// TODO - ���� Transition ������ üũ�մϴ�. (���� ���� ���� ���� ���̴ϱ� ..)
		// TODO - ���� Transition ���̸� ���� üũ ������ �ƴ� �� ����� �����ϸ� �˴ϴ�.

		// Context�� ���� ������Ʈ�� �����ɴϴ�.
		DUOLGameEngine::AnimatorState* currentState = context->_currentStates[0];

		// ���� ������Ʈ�� ��� Ʈ�������� üũ�ؼ� ������ Transition�� �޽��ϴ�.
		DUOLGameEngine::AnimatorStateTransition* transition = currentState->CheckAllTransition(context);

		// Ʈ������ �� �� ���� ��Ȳ�Դϴ�. ���� ������Ʈ�� ���ؼ� Context�� �����մϴ�. 
		if (transition == nullptr)
		{
			NotTransition(context, deltaTime);
		}
		// Ʈ������ �� �� �ִ� ��Ȳ�Դϴ�. ��Transition Duration��, ��Transition Offset�� �� ����
		// ����, ����ǵ��� �����մϴ�. TODO - 1�� 3�� .. �ϴ� ���� ����.
		else
		{
			Transition(transition, context);
		}
	}

	void AnimatorStateMachine::NotTransition(DUOLGameEngine::AnimatorControllerContext* targetContext, float deltaTime)
	{
		// TODO - �ϴ� Ʈ������ ���ɸ� Ȯ���غ��� ..
		targetContext->_animator->Play(deltaTime, targetContext->_currentStates[0]->GetAnimationClip());
	}

	void AnimatorStateMachine::Transition(DUOLGameEngine::AnimatorStateTransition* targetTransition, DUOLGameEngine::AnimatorControllerContext* context)
	{
		// TODO - �ϴ� Ʈ������ ���ɸ� Ȯ���غ��� ..
		context->_currentStates[0] = targetTransition->_to;
	}
}
