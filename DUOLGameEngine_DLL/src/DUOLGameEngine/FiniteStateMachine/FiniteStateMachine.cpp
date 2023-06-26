#include "DUOLGameEngine/FiniteStateMachine/FiniteStateMachine.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"

namespace DUOLGameEngine
{
	FiniteStateMachine::FiniteStateMachine() :
		_states()
		, _currentState(nullptr)
		, _nextState(nullptr)
		, _prevState(nullptr)
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
			std::string from(_currentState->GetName().begin(), _currentState->GetName().end());
			std::string to(nextStatesName.begin(), nextStatesName.end());
			DUOL_TRACE(DUOL_CONSOLE, "FinitieState Transition | Play from {0} -> to {1} ", from, to);
			
			_nextState = _states.at(nextStatesName);

			_currentState->OnStateExit(deltaTime);

			_prevState = _currentState;

			_nextState->OnStateEnter(deltaTime);

			_currentState = _nextState;

			_nextState = nullptr;

			// _prevState = nullptr;
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

	StateBase* FiniteStateMachine::GetPrevState() const
	{
		return _prevState;
	}

	StateBase* FiniteStateMachine::GetCurrentState() const
	{
		return _currentState;
	}

	void FiniteStateMachine::UpdateStateMachine(float deltaTime)
	{
		_currentState->OnStateStay(deltaTime);
	}

	void FiniteStateMachine::LateUpdateStateMachine(float deltaTime)
	{
		_currentState->OnStateStayLate(deltaTime);
	}

	void FiniteStateMachine::FixedUpdateStateMachine(float fixedTimeStep)
	{
		_currentState->OnStateStayFixed(fixedTimeStep);
	}
}