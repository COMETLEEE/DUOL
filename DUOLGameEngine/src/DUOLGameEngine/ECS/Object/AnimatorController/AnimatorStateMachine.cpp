#include <algorithm>

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Object/AnimationClip.h"

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
		// ��� ������Ʈ�� �޸𸮸� �����մϴ�.
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
			_entryState = newState;

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
		if (context->_currentStateMachineContexts[0]._isOnTransition)
		{
			// ���ؽ�Ʈ�� �ش� ������Ʈ �ӽſ��� Ʈ�������� ���� ���̶�� �� �Լ��� ȣ���մϴ�.
			OnTransitionStay(context, deltaTime);

			return;
		}

		// Ʈ�������� ���� ������ �ʴٸ� ���ؽ�Ʈ�� ���� ������Ʈ���� Ʈ������ �������� ���θ� Ȯ���մϴ�.
		// Context�� ���� ������Ʈ�� �����ɴϴ�.
		DUOLGameEngine::AnimatorState* currentState = context->_currentStateContexts[0]._currentState;

		// ���� ������Ʈ�� ��� Ʈ�������� üũ�ؼ� ������ Transition�� �޽��ϴ�.
		// �� ���� ���� ���� Ʈ�������� �����ϴٰ� üũ�� ���� �����ϱ� Order ���� �� ���س����� �������� ..?
		DUOLGameEngine::AnimatorStateTransition* transition = currentState->CheckAllTransition(context);

		// Ʈ������ �� �� ���� ��Ȳ�Դϴ�. ���� ������Ʈ�� ���ؼ� Context�� �����մϴ�. 
		if (transition == nullptr)
			NotTransition(context, deltaTime);
		// Ʈ������ �� �� �ִ� ��Ȳ�Դϴ�. AnimatorControllerContext�� Transition �غ��մϴ�.
		else
			OnTransitionEnter(context, transition);
	}

	void AnimatorStateMachine::NotTransition(DUOLGameEngine::AnimatorControllerContext* targetContext, float deltaTime)
	{
		// ���� ������ �ִϸ��̼��� ��� �÷����մϴ�.
		targetContext->_animator->Play(deltaTime, targetContext->_currentStateContexts[0]._currentState->GetAnimationClip());
	}

	void AnimatorStateMachine::OnTransitionEnter(DUOLGameEngine::AnimatorControllerContext* context, DUOLGameEngine::AnimatorStateTransition* targetTransition)
	{
		// TODO - ����, ���� ������Ʈ�� ������ ��� �������ϰ� Ʈ�������� ���������� ���� üũ�� ������ ���� ����ϵ�.
		context->_currentStateMachineContexts[0]._isOnTransition = true;

		context->_currentTransitionContexts[0]._currentTransition = targetTransition;

		const float transitionOffset = targetTransition->GetTransitionOffset();

		const float transitionDuration = targetTransition->GetTransitionDuration();

		float startFrameOfTo = targetTransition->_to->GetAnimationClip()->GetMaxFrame() * std::clamp<float>(transitionOffset, 0.f, 1.f);

		float remainTime = 0.f;

		// Fixed Duration�̸� �� ���� �ð����� Transition�� �����մϴ�.
		if (!targetTransition->_fixedDuration)
			remainTime = targetTransition->GetTransitionDuration() * targetTransition->_from->GetAnimationClip()->GetTotalPlayTime();
		else
			remainTime = targetTransition->GetTransitionDuration();

		// Transition�� ���� ���� �ʿ��� �������� �����մϴ�.
		// 1. From State�� ���� ������
		// 2. To State�� ���� ������ (transitionOffset�� ���ؼ� ���մϴ�.)
		// 3. Transition�� �� �ð� (�� ����)
		// 4. Transition�� ���� �ð� (���⼭�� 3�� ������ ���� ���̸鼭 ���� ����� �ٲ� �����ϴ�.)
		context->_currentTransitionContexts[0]._currentFrameOfFrom = context->_currentStateContexts[0]._currentFrame;

		context->_currentTransitionContexts[0]._prevFrameOfFrom = context->_currentStateContexts[0]._prevFrame; 

		context->_currentTransitionContexts[0]._currentFrameOfTo = startFrameOfTo;

		// To�� ���� �����ϴ� �༮�̴ϱ� ���� �������� 1���� ���� �������Դϴ�.
		context->_currentTransitionContexts[0]._prevFrameOfTo = std::abs(startFrameOfTo - 1.f);

		context->_currentTransitionContexts[0]._totalTransitionTime = remainTime;

		context->_currentTransitionContexts[0]._remainTransitionTime = remainTime;
	}

	void AnimatorStateMachine::OnTransitionStay(DUOLGameEngine::AnimatorControllerContext* context, float deltaTime)
	{
		DUOLGameEngine::AnimatorStateTransition* transition =  context->_currentTransitionContexts[0]._currentTransition;

		DUOLGameEngine::AnimationClip* fromClip = transition->_from->_animationClip;

		DUOLGameEngine::AnimationClip* toClip = transition->_to->_animationClip;

		// fromClip�� ����� ���� ��� (== �ܿ� �ð� / �� �ð�)
		const float tFrom = std::clamp<float>(context->_currentTransitionContexts[0]._remainTransitionTime / 
			context->_currentTransitionContexts[0]._totalTransitionTime, 0.f, 1.f);

		// Remain Time ����
		context->_currentTransitionContexts[0]._remainTransitionTime -= deltaTime;

		// Transition�� ���� ��� �ð��� ������ ���
		// tFrom�� ���� Ʈ������ ���ؽ�Ʈ�� �󸶳� ������°��� ���� �����˴ϴ�.
		context->_animator->Play(deltaTime, fromClip, toClip, tFrom);

		// Ʈ�����ǿ� ���� �ð��� ������ �����մϴ�.
		if (context->_currentTransitionContexts[0]._remainTransitionTime <= 0.f)
			OnTransitionExit(context, transition);
	}

	void AnimatorStateMachine::OnTransitionExit(DUOLGameEngine::AnimatorControllerContext* context, DUOLGameEngine::AnimatorStateTransition* transition)
	{
		// ���ؽ�Ʈ���� ���� ���¸� �ٲ��ݴϴ�.
		context->_currentStateContexts[0]._currentState = transition->_to;

		// ���� �����ӵ� ���� ToAnimation���� ���� ���� ������ �ٲ��ݴϴ�.
		context->_currentStateContexts[0]._currentFrame = context->_currentTransitionContexts[0]._currentFrameOfTo;

		// ���� �����ӵ� ���� ToAnimation���� ���� �������̿��� ������ �ٲ��ݴϴ�.
		context->_currentStateContexts[0]._prevFrame = context->_currentTransitionContexts[0]._prevFrameOfTo;

		// Ʈ������ ���� ���̶�� �÷��׸� ���ְ�
		context->_currentStateMachineContexts[0]._isOnTransition = false;

		// ���ؽ�Ʈ�� �����մϴ�.
		context->_currentTransitionContexts[0]._currentTransition = nullptr;
		context->_currentTransitionContexts[0]._currentFrameOfFrom = 0.f;
		context->_currentTransitionContexts[0]._prevFrameOfFrom = 0.f;
		context->_currentTransitionContexts[0]._currentFrameOfTo = 0.f;
		context->_currentTransitionContexts[0]._prevFrameOfTo = 0.f;
	}
}