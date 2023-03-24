#include "DUOLGameEngine/FiniteStateMachine/FiniteStateMachine.h"

#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"

namespace DUOLGameEngine
{
	FiniteStateMachine::FiniteStateMachine() :
		_states()
		, _currentState(nullptr)
		, _nextState(nullptr)
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
			_nextState = _states.at(nextStatesName);

			_currentState->OnStateExit(deltaTime);

			_nextState->OnStateEnter(deltaTime);

			_currentState = _nextState;

			_nextState = nullptr;
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

	StateBase* FiniteStateMachine::GetState(const DUOLCommon::tstring& stateName) const
	{
		return _states.contains(stateName) ? _states.at(stateName) : nullptr;
	}

	StateBase* FiniteStateMachine::GetNextState() const
	{
		return _nextState;
	}

	void FiniteStateMachine::UpdateStateMachine(float deltaTime)
	{
		_currentState->OnStateStay(deltaTime);

		// Current State가 바뀌엇을 수 있으니 .. _currentState 호출 금지.
	}

	void FiniteStateMachine::FixedUpdateStateMachine(float fixedTimeStep)
	{
		_currentState->OnStateStayFixed(fixedTimeStep);
	}
}
