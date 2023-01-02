#pragma once
#include <unordered_map>

#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	class AnimatorStateMachine;

	class AnimatorControllerLayer
	{
	public:
		AnimatorControllerLayer(const DUOLCommon::tstring& name);

		~AnimatorControllerLayer();

	private:
		std::unordered_map<DUOLCommon::tstring, DUOLGameEngine::AnimatorStateMachine*> _stateMachines;

	public:
		DUOLGameEngine::AnimatorStateMachine* GetCurrentStateMachine() const;

		void SetCurrentStateMachine(const DUOLCommon::tstring& stateMachineName);

		DUOLGameEngine::AnimatorStateMachine* AddStateMachine(const DUOLCommon::tstring& stateMachineName);

		void RemoveStateMachine(const DUOLCommon::tstring& stateMachineName);

	private:
		float _weight;

		DUOLCommon::tstring _name;

		DUOLGameEngine::AnimatorStateMachine* _currentStateMachine;

		DUOLGameEngine::AnimatorStateMachine* _rootStateMachine;
	};
}