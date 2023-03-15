#include "DUOLGameEngine/FiniteStateMachine/StateBase.h"

namespace DUOLGameEngine
{
	StateBase::StateBase(const DUOLCommon::tstring& stateName) :
		_stateName(stateName)
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
	}

	void StateBase::OnStateExit(float deltaTime)
	{
		_stateExitEvent.Invoke();
	}
}