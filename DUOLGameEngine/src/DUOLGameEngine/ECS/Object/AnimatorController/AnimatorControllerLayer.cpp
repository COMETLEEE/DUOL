#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorControllerLayer.h"

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"

namespace DUOLGameEngine
{
	AnimatorControllerLayer::AnimatorControllerLayer(const DUOLCommon::tstring& name) :
		_name(name)
		, _weight(1.f)
		, _currentStateMachine(nullptr)
	{
		_rootStateMachine = new AnimatorStateMachine(TEXT("RootStateMachine"));

		_currentStateMachine = _rootStateMachine;

		_stateMachines.insert({ _currentStateMachine->GetName(), _currentStateMachine });
	}

	AnimatorControllerLayer::~AnimatorControllerLayer()
	{
		for (auto& [key, value] : _stateMachines)
			delete value;

		_stateMachines.clear();
	}

	DUOLGameEngine::AnimatorStateMachine* AnimatorControllerLayer::GetCurrentStateMachine() const
	{
		return _currentStateMachine;
	}

	void AnimatorControllerLayer::SetCurrentStateMachine(const DUOLCommon::tstring& stateMachineName)
	{
		if (_stateMachines.contains(stateMachineName))
			_currentStateMachine = _stateMachines.at(stateMachineName);
	}

	DUOLGameEngine::AnimatorStateMachine* AnimatorControllerLayer::AddStateMachine(const DUOLCommon::tstring& stateMachineName)
	{
		if (_stateMachines.contains(stateMachineName))
		{
			return AddStateMachine(stateMachineName + TEXT("0"));
		}
		else
		{
			DUOLGameEngine::AnimatorStateMachine* newStateMachine = new AnimatorStateMachine(stateMachineName);

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
}