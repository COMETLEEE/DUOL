#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"

namespace DUOLGameEngine
{
	StateBase::StateBase(const DUOLCommon::tstring& stateName) :
		_stateName(stateName)
		, _isOnStay(false)
	{
	}

	StateBase::~StateBase()
	{
	}

	void StateBase::OnStateEnter(float deltaTime)
	{
		_stateEnterEvent.Invoke();
	}

	void StateBase::OnStateStay(float deltaTime)
	{
		_stateStayEvent.Invoke();

		_isOnStay = true;
	}

	void StateBase::OnStateStayLate(float deltaTime)
	{
		_isOnStay = true;
	}

	void StateBase::OnStateStayFixed(float fixedTimeStep)
	{
		_stateStayFixedEvent.Invoke();

		_isOnStay = true;
	}

	void StateBase::OnStateExit(float deltaTime)
	{
		_stateExitEvent.Invoke();

		_isOnStay = false;
	}
}