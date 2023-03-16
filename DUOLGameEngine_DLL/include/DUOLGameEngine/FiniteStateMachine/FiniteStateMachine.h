#pragma once
#include <map>

#include "DUOLGameEngine/Export_Engine.h"
#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	class StateBase;

	/**
	 * \brief °¡º­¿î Finite State Machine
	 */
	class DUOL_GAMEENGINE_API FiniteStateMachine
	{
	public:
		FiniteStateMachine();

		~FiniteStateMachine();

	private:
		/**
		 * \brief All states in state machine.
		 */
		std::map<DUOLCommon::tstring, StateBase*> _states;

		/**
		 * \brief Current State.
		 */
		StateBase* _currentState;

	public:
		void TransitionTo(StateBase* nextState, float deltaTime);

		void TransitionTo(const DUOLCommon::tstring& nextStatesName, float deltaTime);

		template <typename TState>
		TState* AddState();

		void RemoveState(const DUOLCommon::tstring& stateName);

		void RemoveState(StateBase* state);

	public:
		void UpdateStateMachine(float deltaTime);

		void FixedUpdateStateMachine(float fixedTimeStep);
	};

	template <typename TState>
	TState* FiniteStateMachine::AddState()
	{
		static_assert(std::is_base_of<StateBase, TState>::value, "TState must inherit from StateBase.");
		
		TState* state = new TState();

		state->_stateMachine = this;

		_states.insert({ static_cast<StateBase*>(state)->_stateName, static_cast<StateBase*>(state)});

		// Set entry state.
		if (_currentState == nullptr)
			_currentState = static_cast<StateBase*>(state);

		return state;
	}
}