#include "DUOLGameEngine/FiniteStateMachine/FiniteStateMachine.h"

#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"

namespace DUOLGameEngine
{
	FiniteStateMachine::FiniteStateMachine() :
		_states()
		, _currentState(nullptr)
	{
	}

	FiniteStateMachine::~FiniteStateMachine()
	{
		for (auto [key, value] : _states)
			delete value;
	}

	void FiniteStateMachine::TransitionTo(StateBase* nextState, float deltaTime)
	{
		_currentState->OnStateExit(deltaTime);

		nextState->OnStateEnter(deltaTime);

		_currentState = nextState;
	}

	void FiniteStateMachine::TransitionTo(const DUOLCommon::tstring& nextStatesName, float deltaTime)
	{
		if (_states.contains(nextStatesName))
		{
			auto nextState = _states.at(nextStatesName);

			_currentState->OnStateEnter(deltaTime);

			nextState->OnStateExit(deltaTime);

			_currentState = nextState;
		}
	}

	void FiniteStateMachine::RemoveState(const DUOLCommon::tstring& stateName)
	{
		if (_states.contains(stateName))
			_states.erase(stateName);
	}

	void FiniteStateMachine::RemoveState(StateBase* state)
	{
		for (auto iter = _states.begin(); iter != _states.end() ;)
		{
			if ((*iter).second == state)
			{
				_states.erase(iter);

				break;
			}
			else
				++iter;
		}
	}

	void FiniteStateMachine::UpdateStateMachine(float deltaTime)
	{
		_currentState->OnStateStay(deltaTime);
	}
}