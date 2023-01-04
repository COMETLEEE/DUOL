#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorControllerLayer.h"

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"

namespace DUOLGameEngine
{
	AnimatorControllerLayer::AnimatorControllerLayer(DUOLGameEngine::AnimatorController* animatorController, const DUOLCommon::tstring& name) :
		_name(name)
		, _weight(1.f)
		, _rootStateMachine(nullptr)
		, _currentStateMachine(nullptr)
		, _animatorController(animatorController)
	{
		_rootStateMachine = new AnimatorStateMachine(this,TEXT("RootStateMachine"));

		_currentStateMachine = _rootStateMachine;

		_stateMachines.insert({ _rootStateMachine->GetName(), _rootStateMachine });
	}

	AnimatorControllerLayer::~AnimatorControllerLayer()
	{
		for (auto& [key, value] : _stateMachines)
			delete value;

		_stateMachines.clear();
	}

	DUOLGameEngine::AnimatorStateMachine* AnimatorControllerLayer::AddStateMachine(const DUOLCommon::tstring& stateMachineName)
	{
		if (_stateMachines.contains(stateMachineName))
		{
			return AddStateMachine(stateMachineName + TEXT("0"));
		}
		else
		{
			DUOLGameEngine::AnimatorStateMachine* newStateMachine = new AnimatorStateMachine(this, stateMachineName);

			_stateMachines.insert({ newStateMachine->GetName(), newStateMachine });

			return newStateMachine;
		}
	}

	void AnimatorControllerLayer::RemoveStateMachine(const DUOLCommon::tstring& stateMachineName)
	{
		if (stateMachineName == TEXT("RootStateMachine"))
			return;

		if (_stateMachines.contains(stateMachineName))
		{
			// 스테이트 머신을 해제합니다.
			delete _stateMachines.at(stateMachineName);

			// 그리고 unordered_map에서 제거합니다.
			_stateMachines.erase(stateMachineName);
		}
	}

	DUOLGameEngine::AnimatorStateMachine* AnimatorControllerLayer::GetRootStateMachine() const
	{
		return _rootStateMachine;
	}

	DUOLGameEngine::AnimatorStateMachine* AnimatorControllerLayer::GetCurrentStateMachine() const
	{
		return _currentStateMachine;
	}

	void AnimatorControllerLayer::SetCurrentStateMachine(DUOLGameEngine::AnimatorStateMachine* stateMachine)
	{
		_currentStateMachine = stateMachine;
	}

	void AnimatorControllerLayer::UpdateAnimatorControllerLayer(DUOLGameEngine::AnimatorControllerContext* context, float deltaTime)
	{
		// Layer 별 해당 컨텍스트에서 진행 중인 State machine, State 에 대해서 업데이트를 진행한다.
		context->_currentStateMachineContexts[0]._currentStateMachine->UpdateAnimatorStateMachine(context, deltaTime);
	}
}